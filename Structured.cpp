//
// Created by Franco Yang on 23/05/16.
//

#include <iostream>
#include <opencv2/opencv.hpp>

#include "TimeProfiler.h"

using namespace cv;
using namespace std;

#define INDEX(row, col, cols) (col + row * cols)



inline uchar* newImg(int rows, int cols, uchar initval = 0);
inline uchar* cloneImg(const uchar* img, int rows, int cols);
uchar* immerge(const uchar* img, int rows, int cols, int paddingTop , int paddingLeft , int initValue);
void dilation(uchar*& img, int rows, int cols, const uchar* SE, int seRows, int seCols);
void erosion(uchar*& img, int rows, int cols, const uchar* SE, int seRows, int seCols);
void imshow(string message, uchar* img, int rows, int cols);

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
    uchar* img = imgCV.data;


    int seRows = 7;
    int seCols = 7;
    #define RECTANGLE_KERNEL_OPTIMIZATION
    // se definito, viene ottimizzata la compilazione di erosion e dilation per supportare solo kernel rettangolari.


    const uchar* SE =  newImg(seRows, seCols, 1);

    TimeProfiler t;

    t.start();
    erosion(img, m, n, SE, seRows, seCols);
    t.stop();
    cout << "Erosion: " << t << endl;
    //imshow("erodedImg", img, m, n);

    t.start();
    erosion(img, m, n, SE, seRows, seCols);
    t.stop();
    cout << "Erosion: " << t << endl;

    t.start();
    erosion(img, m, n, SE, seRows, seCols);
    t.stop();
    cout << "Erosion: " << t << endl;


    t.start();
    for( int i = 0; i < 50; i++ )
    {
        erosion(img, m, n, SE, seRows, seCols);
    }
    t.stop();
    cout << "Bench (erosion x 100): " << t << endl;


//
//    waitKey

    delete[] SE;

    return 0;
}







































void imshow(string message,  uchar* img, int rows, int cols){
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
uchar* immerge(const uchar* img, int rows, int cols, int paddingTop , int paddingLeft , int initValue)
{
    const int immergRows = rows + 2*paddingTop;
    const int immergCols = cols + 2*paddingLeft;

    uchar* immergedImg = newImg(immergRows, immergCols, initValue);

    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < cols; j++)
            immergedImg[INDEX( i + paddingTop , j + paddingLeft, immergCols) ] = img[INDEX(i, j, cols) ];

    }

    return immergedImg;
}


//#define SUPER_OPT // in debug riesco a guadagnare circa 1 o mezzo secondo.. (su 50 iterazioni, circa 24.5 - 25 secondi)
                    //in release perdo 2 secondi (su 100 iterazioni, circa 18 secondi)



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

#pragma omp parallel for num_threads(rows/8)
    for(int y = 0; y < rows; y++)
    {
        for(int x = 0; x < cols; x++)
        {
            uchar min = immergedImg[ INDEX(y + paddingTop, x + paddingLeft, cols) ];


            for(int i = 0; i < seRows; i++)
            {

                #ifdef SUPER_OPT
                xj_row_index = x + (y+i) * cols;
                xj_row_index_limit = xj_row_index + seCols;
                for( ; xj_row_index < xj_row_index_limit; xj_row_index++)
                #else
                const int yi = y+i;
                for(int j = 0; j < seCols; j++)
                #endif
                {

                #ifndef RECTANGLE_KERNEL_OPTIMIZATION
                    if(SE[INDEX(i,j, seCols)]  == 1)
                    {
                #endif
                    #ifdef SUPER_OPT
                    const uchar current = immergedImg[xj_row_index];
                    #else
                    const uchar& current = immergedImg[INDEX(yi, x+j, cols)];
                    #endif
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

    int paddingTop = floor(seCols/2);
    int paddingLeft = floor(seRows/2);

    uchar* immergedImg = immerge(img, rows, cols, paddingTop, paddingLeft, 0);

#pragma omp parallel for num_threads(rows/8)
    for(int y = 0; y < rows; y++)
    {
        for(int x = 0; x < cols; x++)
        {
            uchar max = immergedImg[ INDEX(y + paddingTop, x + paddingLeft, cols) ];

            for(int i = 0; i < seRows; i++)
            {


                #ifdef SUPER_OPT
                    const int rowIndex = (y+i) * cols;
                    int xj_row_index = x + rowIndex;
                    for(int j = 0; j < seCols; xj_row_index++, j++)
                #else
                    const int yi = y+i;
                    for(int j = 0; j < seCols; j++)
                #endif
                    {

                    #ifndef RECTANGLE_KERNEL_OPTIMIZATION
                        if(SE[INDEX(i,j, seCols)]  == 1)
                        {
                    #endif
                    #ifdef SUPER_OPT
                        const uchar current = immergedImg[xj_row_index];
                    #else
                        const uchar current = immergedImg[INDEX(yi, x+j, cols)];
                    #endif
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