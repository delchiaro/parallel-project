//
// Created by Franco Yang on 23/05/16.
//

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "utils/TimeProfiler.h"

using namespace cv;
using namespace std;

#define INDEX(row, col, cols) ((col) + (row) * (cols))



inline uchar* newImg(int rows, int cols, uchar initval = 0);
inline uchar* cloneImg(const uchar* img, int rows, int cols);
uchar* immerge(const uchar* img, int rows, int cols, int paddingTop , int paddingLeft , uchar initValue);
void dilation(uchar*& img, int rows, int cols, const uchar* SE, int seRows, int seCols);
void erosion(uchar*& img, int rows, int cols, const uchar* SE, int seRows, int seCols);
void imshow(string message, uchar* img, int rows, int cols);

void blockErosion(uchar*& img, int rows, int cols, const uchar* SE, int seRows, int seCols, int blockRows, int blockCols);



// se definito, viene ottimizzata la compilazione di erosion e dilation per supportare solo kernel rettangolari.
#define RECTANGLE_KERNEL_OPTIMIZATION



// DEFINE BLOCK_EROSION TO ENABLE BLOCK_EROSION INSTEAD OF STANDARD EROSION
// #define BLOCK_EROSION
int blockDim = 4;
int nThreads = 32;

int seRows = 7;
int seCols = 7;

int nErosions = 30;



int main(int argc, char** argv){

    if ( argc != 2 )
    {
        printf("Usage: DisplayImage.out <Image_Path>\n");
        return -1;
    }

    cout << "OpenMP version: " << _OPENMP << endl;
    cout << "Structured Version. " << endl;

    Mat imgCV = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
    int m = imgCV.rows;
    int n = imgCV.cols;
    uchar* originalImage = imgCV.data;
    uchar* processedImage = cloneImg(originalImage,m,n);


    const uchar* SE =  newImg(seRows, seCols, 1);

    TimeProfiler t;


    uchar* multipleProcessedImg = cloneImg(originalImage,m,n);

#ifdef BLOCK_EROSION


    t.start();
    blockErosion(processedImage, m, n, SE, seRows, blockDim, blockDim, blockDim);
    t.stop();
    cout << "Erosion: " << t << endl;

    t.start();
    for( int i = 0; i < nErosions; i++ )
        blockErosion(multipleProcessedImg, m, n, SE, seRows, blockDim, blockDim, blockDim);
    t.stop();
    cout << "Erosion IConvBench (erosion x " << nErosions << "): " << t << endl;

#else


    t.start();
    erosion(processedImage, m, n, SE, seRows, seCols);
    t.stop();
    cout << "Erosion: " << t << endl;

    t.start();
    for( int i = 0; i < nErosions; i++ )
    {
        erosion(multipleProcessedImg, m, n, SE, seRows, seCols);
    }
    t.stop();
    cout << "Erosion IConvBench (erosion x " << nErosions << "): " << t << endl;

#endif




    imshow("Original Image", originalImage, m, n);
    imshow("Processed Image", processedImage, m, n);
    cv::waitKey(0);

    delete[] SE;

    return 0;
}







































void imshow(string message, uchar* img, int rows, int cols){
    cv::Mat cvImg(rows, cols, CV_8U, img);
    imshow(message, cvImg);
}

inline uchar* newImg(int rows, int cols, uchar initval)
{
    uchar* img = new uchar[rows * cols];
    for(int i = 0; i < rows * cols ; i++)
        img[i] = initval;
    return img;
}

inline uchar* cloneImg(const uchar* img, int rows, int cols)
{
    uchar* clone = new uchar[rows * cols];
    for(int i = 0; i < rows * cols ; i++)
        clone[i] = img[i];
    return clone;
}

// Si assume un kernel con dimensioni m*n, con m,n dispari
uchar* immerge(const uchar* img, int rows, int cols, int paddingTop , int paddingLeft , uchar initValue)
{
    const int immergRows = rows + 2*paddingTop;
    const int immergCols = cols + 2*paddingLeft;

    uchar* immergedImg = newImg(immergRows, immergCols, initValue);

    for(long i = 0; i < rows; i++)
    {
        for(long j = 0; j < cols; j++)
        {
            // NB: using INDEX( i+paddingTop, j+paddingLeft, immergCols ) will not works because MACRO will be translated as:
            // (col + row * cols) ----> i+paddingTop + j+paddingLeft  * immergCols --->   i + paddingTop + j + ( paddingLeft  * immergCols )  :(
            immergedImg[INDEX( i+paddingTop, j+paddingLeft, immergCols)] = img[INDEX(i, j, cols)];
        }

    }

    return immergedImg;
}



// la computazione viene eseguita su immergedImg e viene scritto il risultato in img
void dilation(uchar*& img, int rows, int cols, const uchar* SE, int seRows, int seCols)
{

    int paddingTop = floor(seCols/2);
    int paddingLeft = floor(seRows/2);

    uchar* immergedImg = immerge(img, rows, cols, paddingTop, paddingLeft, 255);
    #ifdef SUPER_OPT
        int xj_row_index = 0;
        int xj_row_index_limit=0;
    #endif

#pragma omp parallel for num_threads(nThreads)
    for(int y = 0; y < rows; y++)
    {
        for(int x = 0; x < cols; x++)
        {
            uchar min = immergedImg[ INDEX(y + paddingTop, x + paddingLeft, cols) ];


            for(int i = 0; i < seRows; i++)
            {

                const int yi = y+i;
                for(int j = 0; j < seCols; j++)
                {

                #ifndef RECTANGLE_KERNEL_OPTIMIZATION
                    if(SE[INDEX(i,j, seCols)]  == 1)
                    {
                #endif
                    const uchar& current = immergedImg[INDEX(yi, x+j, cols)];
                    if (current < min)
                        min = current;

                #ifndef RECTANGLE_KERNEL_OPTIMIZATION
                    }
                #endif
                }
            }

            img[INDEX(y, x, cols)] = min;
        }
    }
    delete[] immergedImg;

    return ;
}



// la computazione viene eseguita su immergedImg e viene scritto il risultato in img
void erosion(uchar*& img, int rows, int cols, const uchar* SE, int seRows, int seCols)
{

    int paddingTop = ceil(seCols/2);
    int paddingLeft = ceil(seRows/2);

    uchar* immergedImg = immerge(img, rows, cols, paddingTop, paddingLeft, 255);
    int immergedRows = rows + 2*paddingTop;
    int immergedCols = cols + 2*paddingLeft;

#pragma omp parallel for num_threads(nThreads)
    for(int y = 0; y < rows; y++)
    {
        for(int x = 0; x < cols; x++)
        {
            uchar max = immergedImg[ INDEX( y + paddingTop, x + paddingLeft, immergedCols) ];

            for(int i = 0; i < seRows; i++)
            {

                const int yi = y+i;
                for(int j = 0; j < seCols; j++)
                {

                #ifndef RECTANGLE_KERNEL_OPTIMIZATION
                    if(SE[INDEX(i,j, seCols)]  == 1)
                    {
                #endif

                    const uchar current = immergedImg[INDEX( yi, x+j, immergedCols)];
                    if (current < max)
                        max = current;

                #ifndef RECTANGLE_KERNEL_OPTIMIZATION
                    }
                #endif

                }

            }

            img[INDEX(y, x, cols)] = max;
        }
    }

    delete[] immergedImg;
}





// la computazione viene eseguita su immergedImg e viene scritto il risultato in img
void blockErosion(uchar*& img, int rows, int cols, const uchar* SE, int seRows, int seCols, int blockRows, int blockCols)
{
    /*  Esempio di una immagine 11x11 divisa in 4 blocchi riga e 4 blocchi colonna
     *  I blocchi centrali avranno dimensione block_dim_rows e block_dim_cols (3x3)
     *  I blocchi finali sul basso avranno dimensione fvBlock_dim_...
     *  I blocchi finali sulla destra avranno dimensione fhBlock_dim_...
     *  Il blocco finale in basso a destra avrá dimensione ffBlock_dim...
     *  __________ __________ __________ _______
     *  |__|__|__| |__|__|__| |__|__|__| |__|__|
     *  |__|__|__| |__|__|__| |__|__|__| |__|__|
     *  |__|__|__| |__|__|__| |__|__|__| |__|__|
     *  __________ __________ __________ _______
     *  |__|__|__| |__|__|__| |__|__|__| |__|__|
     *  |__|__|__| |__|__|__| |__|__|__| |__|__|
     *  |__|__|__| |__|__|__| |__|__|__| |__|__|
     *  __________ __________ __________ _______
     *  |__|__|__| |__|__|__| |__|__|__| |__|__|
     *  |__|__|__| |__|__|__| |__|__|__| |__|__|
     *  |__|__|__| |__|__|__| |__|__|__| |__|__|
     *  __________ __________ __________ _______
     *  |__|__|__| |__|__|__| |__|__|__| |__|__|
     *  |__|__|__| |__|__|__| |__|__|__| |__|__|
     *
     *
     */
    int paddingTop = ceil(seCols/2);
    int paddingLeft = ceil(seRows/2);

    int block_dim_rows = rows / blockRows;
    int block_dim_cols = cols / blockCols;


    int fhBlock_dim_rows = rows % blockRows; // final horizontal block (bottom blocks)
    int fhBlock_dim_cols = block_dim_cols;
    bool noFvBlock = (fhBlock_dim_rows == 0);

    int fvBlock_dim_rows = block_dim_rows;
    int fvBlock_dim_cols = cols % block_dim_cols;// final vertical block (right blocks)
    bool noFhBlock = (fvBlock_dim_cols == 0);

    int ffBlock_dim_rows = fhBlock_dim_rows; // final final block (bottom right corner).
    int ffBlock_dim_cols = fvBlock_dim_cols;
    bool noFfBlock = (noFvBlock || noFhBlock);

    uchar* immergedImg = immerge(img, rows, cols, paddingTop, paddingLeft, 255);
    //int immergedWidth = rows + 2*paddingTop;
    int immergedCols = cols + 2*paddingLeft;


#pragma omp parallel sections
    {

#pragma omp section

    #pragma omp parallel for  num_threads(blockRows -1 + noFvBlock)
    for(int bi = 0 ; bi < blockRows -1 + noFvBlock ; bi++)
    {
        #pragma omp parallel for  num_threads(blockCols -1 + noFhBlock)
        for(int bj = 0 ; bj < blockCols -1 + noFhBlock ; bj++)
        {
            // for each BLOCK(bi, bj):

                const int block_dim_row_stop = (bi+1)*block_dim_rows;
                for(int y = bi*block_dim_rows; y < block_dim_row_stop; y++)
                {
                    const int block_dim_col_stop = (bj+1)*block_dim_cols;
                    for(int x = bj*block_dim_cols; x < block_dim_col_stop; x++)
                    {
                        uchar max = immergedImg[ INDEX( (y+paddingTop), (x+paddingLeft), immergedCols) ];

                        for(int i = 0; i < seRows; i++)
                        {

                            const int yi = y+i;
                            for(int j = 0; j < seCols; j++)
                            {

                                #ifndef RECTANGLE_KERNEL_OPTIMIZATION
                                if(SE[INDEX(i,j, seCols)]  == 1)
                                {
                                #endif

                                const uchar current = immergedImg[INDEX(yi, (x+j), immergedCols)];
                                if (current < max)
                                    max = current;

                                #ifndef RECTANGLE_KERNEL_OPTIMIZATION
                                }
                                #endif

                            }

                        }

                        img[INDEX(y, x, cols)] = max;
                    }
                }



        }
    }



#pragma omp section

    // BLOCCHI ORIZZONTALI FINALI IN BASSO
    if(noFhBlock == false)
    {
        int bi =  blockRows-1;
        #pragma omp parallel for  num_threads(blockCols -1 + noFhBlock)
        for(int bj = 0 ; bj < blockCols -1 + noFvBlock ; bj++)
        {
            // for each BLOCK(bi, bj):

            const int fhBlock_dim_row_stop = (bi+1)*block_dim_rows + fhBlock_dim_rows;
            for(int y = bi*fhBlock_dim_rows; y < fhBlock_dim_row_stop; y++)
            {
                const int fhBlock_dim_col_stop = (bj+1)*fhBlock_dim_cols + fhBlock_dim_cols;
                for(int x = bj*fhBlock_dim_cols; x < fhBlock_dim_col_stop; x++)
                {
                    uchar max = immergedImg[ INDEX(y+ paddingTop, x + paddingLeft, immergedCols) ];

                    for(int i = 0; i < seRows; i++)
                    {

                        const int yi = y+i;
                        for(int j = 0; j < seCols; j++)
                        {

                            #ifndef RECTANGLE_KERNEL_OPTIMIZATION
                            if(SE[INDEX(i,j, seCols)]  == 1)
                            {
                            #endif

                            const uchar current = immergedImg[INDEX(yi, x+j, immergedCols)];
                            if (current < max)
                                max = current;

                            #ifndef RECTANGLE_KERNEL_OPTIMIZATION
                            }
                            #endif

                        }

                    }

                    img[INDEX(y, x, cols)] = max;
                }
            }



        }
    }

#pragma omp section

    // BLOCCHI VERTICALI FINALI SULLA DESTRA
    if(noFvBlock == false)
    {
        int bj =  blockCols-1;
        #pragma omp parallel for  num_threads(blockRows -1 + noFvBlock)
        for(int bi = 0 ; bi < blockRows -1 + noFhBlock; bi++)
        {
            // for each BLOCK(bi, bj):

            const int fvBlock_dim_row_stop = (bi+1)*block_dim_rows + fvBlock_dim_rows;
            for(int y = bi*fvBlock_dim_rows; y < fvBlock_dim_row_stop; y++)
            {
                const int fvBlock_dim_col_stop = (bj+1)*block_dim_cols + fvBlock_dim_cols;
                for(int x = bj*fvBlock_dim_cols; x < fvBlock_dim_col_stop; x++)
                {
                    uchar max = immergedImg[ INDEX(y + paddingTop, x + paddingLeft, immergedCols) ];

                    for(int i = 0; i < seRows; i++)
                    {

                        const int yi = y+i;
                        for(int j = 0; j < seCols; j++)
                        {

                            #ifndef RECTANGLE_KERNEL_OPTIMIZATION
                            if(SE[INDEX(i,j, seCols)]  == 1)
                            {
                            #endif

                            const uchar current = immergedImg[INDEX(yi, x+j, immergedCols)];
                            if (current < max)
                                max = current;

                            #ifndef RECTANGLE_KERNEL_OPTIMIZATION
                            }
                            #endif

                        }

                    }

                    img[INDEX(y, x, cols)] = max;
                }
            }



        }
    }

#pragma omp section

    // BLOCCO FINALE IN BASSO A DESTRA
    if(noFfBlock == false)
    {
        int bj =  blockCols-1;
        int bi = blockRows-1;

        const int ffBlock_dim_row_stop = bi*block_dim_rows + ffBlock_dim_rows;
        for(int y = bi*ffBlock_dim_rows; y < ffBlock_dim_row_stop; y++)
        {
            const int ffBlock_dim_col_stop = bj*block_dim_cols + ffBlock_dim_cols;
            for(int x = bj*ffBlock_dim_cols; x < ffBlock_dim_col_stop; x++)
            {
                uchar max = immergedImg[ INDEX(y + paddingTop, x + paddingLeft, immergedCols) ];

                for(int i = 0; i < seRows; i++)
                {

                    const int yi = y+i;
                    for(int j = 0; j < seCols; j++)
                    {

                        #ifndef RECTANGLE_KERNEL_OPTIMIZATION
                        if(SE[INDEX(i,j, seCols)]  == 1)
                        {
                        #endif

                        const uchar current = immergedImg[INDEX(yi, x+j, immergedCols)];
                        if (current < max)
                            max = current;

                        #ifndef RECTANGLE_KERNEL_OPTIMIZATION
                        }
                        #endif

                    }

                }

                img[INDEX(y, x, cols)] = max;
            }
        }



    }


    }

    delete[] immergedImg;
}