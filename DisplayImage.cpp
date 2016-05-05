#include <iostream>
#include <stdio.h>
#include <opencv2/opencv.hpp>



using namespace std;
using namespace cv;

class Matrix
{
private:
    int columns;
    int rows;
    uchar* mat;

public:
    Matrix(int rows, int cols) : columns(rows), rows(cols)
    {
        mat = new uchar[rows * cols];
    }

    ~Matrix()
    {
        delete mat;
    }


};

Mat dilation(Mat img, const Mat SE, const cv::Point2i SE_center)
{
    //CV_Assert(bw_SE.depth() == CV_8U);  // accept only uchar Structuring Elements
    //CV_Assert(img.depth() == CV_8U);  // accept only uchar images

    Mat output = img.clone();

    for(int y = 0; y < img.rows; y++)
    {

        for(int x = 0; x < img.cols; x++)
        {




            uchar min = *(img.row(y).col(x).data);
            for(int i = 0; i < SE.rows; i++)
            {
                for (int j = 0; j < SE.cols; j++)
                {
                    if( (y-SE_center.y+i) >=0 && (x-SE_center.x+j) >= 0 && (y-SE_center.y+i) < img.rows && (x-SE_center.x+j) < img.cols)
                    { // CHECK if SE is inside the image edges

                        if (*(SE.row(i).col(j).data) >= 1)
                        { // if SE[i][j]==1, apply the mask:

                            const uchar *current = img.row(y - SE_center.y + i).col(x - SE_center.x + j).data;
                            if (*current < min)
                                min = *current;
                        }
                    }
                }
            }




            uchar *current = output.row(y).col(x).data;
            *current = min;
        }
    }
    return output;
}


Mat erosion(Mat img, const Mat bw_SE, const cv::Point2i SE_center)
{
    //CV_Assert(bw_SE.depth() == CV_8U);  // accept only uchar Structuring Elements
    //CV_Assert(img.depth() == CV_8U);  // accept only uchar images

    Mat output = img.clone();


    for(int y = 0; y < img.rows; y++)
    {

        for(int x = 0; x < img.cols; x++)
        {


            uchar max = *(img.row(y).col(x).data);
            for(int i = 0; i < bw_SE.rows; i++)
            {
                for (int j = 0; j < bw_SE.cols; j++)
                {
                    if( (y-SE_center.y+i) >=0 && (x-SE_center.x+j) >= 0 && (y-SE_center.y+i) < img.rows && (x-SE_center.x+j) < img.cols)
                    {
                        if (*(bw_SE.row(i).col(j).data) >= 1)
                        { // lo SE vale 1, quindi applichiamo la maschera.

                            const uchar *current = img.row(y - SE_center.y + i).col(x - SE_center.x + j).data;
                            if (*current > max)
                                max = *current;
                        }
                    }
                }
            }

            uchar *current = output.row(y).col(x).data;
            *current = max;
        }
    }
    return output;
}



Mat opening(Mat img, const Mat bw_SE, const cv::Point2i SE_center)
{
    Mat er = erosion(img, bw_SE, SE_center);
    return dilation(er, bw_SE, SE_center);
}

Mat closure(Mat img, const Mat bw_SE, const cv::Point2i SE_center)
{
    Mat dil = dilation(img, bw_SE, SE_center);
    return erosion(dil, bw_SE, SE_center);
}



int main(int argc, char** argv ) {
    if ( argc != 2 )
    {
        printf("usage: DisplayImage.out <Image_Path>\n");
        return -1;
    }

    Mat image = imread( argv[1], 0); //CV_LOAD_IMAGE_GRAYSCALE);

    if ( !image.data )
    {
        printf("No image data \n");
        return -1;
    }
    //namedWindow("Original Image", WINDOW_AUTOSIZE );
    imshow("Original Image", image);



    int rows = 6;
    int cols = 6;
    Mat2i SE(cols, rows, CV_8U);
    SE = Mat::ones(cols, rows, CV_8U);
    Point2i center(ceil(cols/2), ceil(rows/2));

    Mat open = opening(image, SE, center );
    //namedWindow("Opened Image", WINDOW_AUTOSIZE );
    imshow("Opened Image", open);


    Mat closed = closure(image, SE, center );
    //namedWindow("Closed Image", WINDOW_AUTOSIZE );
    imshow("Closed Image", closed);

    Mat closedOpened = closure(open, SE, center);
    imshow("Closed Opened Image", closedOpened);


    waitKey(0);

    return 0;
}


