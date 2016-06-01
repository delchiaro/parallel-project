//
// Created by Franco Yang on 23/05/16.
//

#include <iostream>
#include <opencv2/opencv.hpp>

#include "TimeProfiler.h"

using namespace cv;
using namespace std;

// Si assume un kernel con dimensioni m*n, con m,n dispari
Mat immerge(const Mat& img , int paddingTop , int paddingLeft , int initValue){

    Mat immergedImg;

    immergedImg = Mat(img.rows + 2*paddingTop , img.cols + 2*paddingLeft , CV_8UC1, initValue);

    for(int i = 0; i < img.rows; i++)
    {
        for(int j = 0; j < img.cols; j++)
        {
            immergedImg.at<uchar>(i + paddingTop , j + paddingLeft) = img.at<uchar>(i,j);
        }
    }

    return immergedImg;

};

// In entrambe le funzioni la computazione viene eseguita su immergedImg e viene scritto il risultato in img
Mat dilation(Mat& img , const Mat& SE){

    int paddingTop = floor(SE.cols/2);
    int paddingLeft = floor(SE.rows/2);

    Mat immergedImg = immerge(img, paddingTop, paddingLeft, 255);

    for(int y = 0; y < img.rows; y++)
    {
        for(int x = 0; x < img.cols; x++)
        {
            uchar min = immergedImg.at<uchar>(y + paddingTop, x + paddingLeft);

            for(int i = 0; i < SE.rows; i++)
            {
                for(int j = 0; j < SE.cols; j++)
                {
                    if(SE.at<bool>(i,j) == 1)
                    {
                        const uchar& current = immergedImg.at<uchar>(y+i, x+j);
                        if (current < min)
                            min = current;
                    }
                }
            }

            img.at<uchar>(y,x) = min;
        }
    }

    return img;

};

Mat erosion(Mat& img , const Mat& SE){

    int paddingTop = floor(SE.cols/2);
    int paddingLeft = floor(SE.rows/2);

    Mat immergedImg = immerge(img, paddingTop, paddingLeft, 0);

    #pragma omp parallel for num_threads(img.rows/8)
    for(int y = 0; y < img.rows; y++)
    {
        for(int x = 0; x < img.cols; x++)
        {
            uchar max = immergedImg.at<uchar>(y + paddingTop, x + paddingLeft);

            for(int i = 0; i < SE.rows; i++)
            {
                for(int j = 0; j < SE.cols; j++)
                {
                    if(SE.at<bool>(i,j) == true)
                    {
                        const uchar& current = immergedImg.at<uchar>(y+i, x+j);
                        if (current > max)
                            max = current;
                    }
                }
            }

            img.at<uchar>(y, x) = max;
        }
    }

    return img;

};

int main(int argc, char** argv){

    if ( argc != 2 )
    {
        printf("Usage: DisplayImage.out <Image_Path>\n");
        return -1;
    }

    cout << "OpenMP version: " << _OPENMP << endl;
    cout << "Structured Version. " << endl;

    Mat img = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);

    int rows = 7;
    int cols = 7;

    const Mat SE = Mat(rows, cols, CV_8U, true);

    TimeProfiler t;

    Mat eroded;
    t.start();
    eroded = erosion(img, SE);
    t.stop();
    cout << "Erosion: " << t << endl;

    Mat eroded1;
    t.start();
    eroded1 = erosion(img, SE);
    t.stop();
    cout << "Erosion: " << t << endl;

    Mat eroded2;
    t.start();
    eroded2 = erosion(img, SE);
    t.stop();
    cout << "Erosion: " << t << endl;


    Mat bench;
    t.start();
    for( int i = 0; i < 10; i++ )
    {
        erosion(img, SE);
    }
    t.stop();
    cout << "Bench (erosion x 100): " << t << endl;


//    imshow("Eroded Image", eroded);
//
//    waitKey(0);

}