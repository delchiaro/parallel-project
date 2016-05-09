//
// Created by Nagash on 09/05/16.
//

#ifndef DISPLAYIMAGE_IMAGE_H
#define DISPLAYIMAGE_IMAGE_H

#include <iostream>
#include "Matrix.h"
#include <opencv2/opencv.hpp>



namespace imProc
{



template <typename T> class Image : Matrix<T>{
private:

protected:

public:


    Image(cv::Mat openCV_image) : Matrix(openCV_image.rows, openCV_image.cols)
    {

        for(int i = 0; i < openCV_image.rows; i++)
            for(int j = 0; j < openCV_image.cols; j++)
                this->set(i,j, openCV_image.row(i).col(j));
    }

    static cv::Mat readImageCV(std::string path)
    {
        using namespace cv;
        Mat image = imread( path, 0); //CV_LOAD_IMAGE_GRAYSCALE);
        if ( !image.data )
        {
            // printf("No image data \n");
        }
        return image;
    }





};

}

#endif //DISPLAYIMAGE_IMAGE_H
