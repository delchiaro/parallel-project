//
// Created by Nagash on 09/05/16.
//

#ifndef DISPLAYIMAGE_IMAGE_H
#define DISPLAYIMAGE_IMAGE_H


#include "imProc.h"

#include "Matrix.h"
#include "point.h"
#include <opencv2/opencv.hpp>
#include <iostream>

namespace imProc
{


    template <typename T> class Image : public Matrix<T>{
    private:

    protected:

    public:


        Image( const Image& copy ) : Matrix<T>(copy) {} // copy constructor

        Image(cv::Mat openCV_image) : Matrix<T>(openCV_image.rows, openCV_image.cols)
        {
            for(int i = 0; i < openCV_image.rows; i++)
                for(int j = 0; j < openCV_image.cols; j++)
                    this->set(i,j, openCV_image.col(j).row(i).data);
        }

        Image(int rows, int cols, const T& initVal) : Matrix<T>(rows, cols, initVal) { }

        Image<T> clone() const { return Image(*this); }



        inline  cv::Mat toMatCV() const {
            return cv::Mat(this->rows(), this->cols(), CV_8U, (void*) this->getRawMatrix() , 0);
            // NB: We hope in Return Value Optimization (see wikipedia)
        }
        inline cv::Mat toMatCV_for() const
        {
            cv::Mat cvMat =  cv::Mat( cv::Size(this->rows(), this->cols()), 0);
            for(int i = 0; i < this->rows(); i++)
                for(int j = 0; j < this->cols(); j++)
                    cvMat.row(i).col(j).data = this->get(i, j);
            return cvMat;
            // NB: We hope in Return Value Optimization (see wikipedia)
        }



        inline void imshow(std::string msg = "") {
            cv::Mat m = toMatCV();
            cv::imshow(msg, m);
            //delete m;
        }


        inline Image<T> dilation(const Matrix<bool>& SE, const Point<int>& SE_center) {
            return dilation(*this, SE, SE_center);
        }
        inline Image<T> erosion(const Matrix<bool>& SE, const Point<int>& SE_center) {
            return erosion(*this, SE, SE_center);
        }
        inline Image<T> opening(const Matrix<bool>& SE, const Point<int>& SE_center) {
            return opening(*this, SE, SE_center);
        }
        inline Image<T> closure(const Matrix<bool>& SE, const Point<int>& SE_center) {
            return closure(*this, SE, SE_center);
        }



// * * * static:      * * *

    protected:
        static cv::Mat _readImageCV(std::string path)
        {
            using namespace cv;
            Mat image = imread( path, CV_LOAD_IMAGE_GRAYSCALE); //CV_LOAD_IMAGE_GRAYSCALE);
            if ( !image.data )
            {
                // printf("No image data \n");
                // TODO: generate exception
            }
            return image;
        }


    public:
        static Image<T> newImageCV(std::string path) {
            return Image(_readImageCV(path));
        }






        static Image<T> dilation(const Image<T>& img, const Matrix<bool>& SE, const Point<int>& SE_center)
        {
            //CV_Assert(bw_SE.depth() == CV_8U);  // accept only uchar Structuring Elements
            //CV_Assert(img.depth() == CV_8U);  // accept only uchar images

            Image output = img.clone();

            for (int y = 0; y < img.rows(); y++) {
                for (int x = 0; x < img.cols(); x++) {


                    uchar min = img.get(y, x);
                    for (int i = 0; i < SE.rows(); i++) {
                        for (int j = 0; j < SE.cols(); j++) {
                            if( (y - SE_center.y() + i) >= 0 && (x - SE_center.x() + j) >= 0 &&
                                (y - SE_center.y() + i) < img.rows() && (x - SE_center.x() + j) < img.cols())
                            { // CHECK if SE is inside the image edges

                                if (SE.get(i, j) == true)// if SE[i][j]==1, apply the mask:
                                {
                                    const uchar *current = &img.get(y - SE_center.y() + i, x - SE_center.x() + j);
                                    if (*current < min)
                                        min = *current;
                                }
                            }
                        }
                    }

                    output.set(y, x, min);
                }
            }
            return output;
        }


        static Image<T> erosion(const Image<T>& img, const Matrix<bool>& SE, const Point<int>& SE_center)
        {
            //CV_Assert(bw_SE.depth() == CV_8U);  // accept only uchar Structuring Elements
            //CV_Assert(img.depth() == CV_8U);  // accept only uchar images

            Image output = img.clone();

            for (int y = 0; y < img.rows(); y++) {
                for (int x = 0; x < img.cols(); x++) {


                    uchar max = img.get(y, x);
                    for (int i = 0; i < SE.rows(); i++) {
                        for (int j = 0; j < SE.cols(); j++) {
                            if ((y - SE_center.y() + i) >= 0 && (x - SE_center.x() + j) >= 0 &&
                                (y - SE_center.y() + i) < img.rows() &&
                                (x - SE_center.x() + j) < img.cols()) { // CHECK if SE is inside the image edges

                                if (SE.get(i, j) == true)// if SE[i][j]==1, apply the mask:
                                {
                                    const uchar *current = &img.get(y - SE_center.y() + i, x - SE_center.x() + j);
                                    if (*current > max)
                                        max = *current;
                                }
                            }
                        }
                    }

                    output.set(y, x, max);
                }
            }
            return output;
        }


        static inline Image<T> opening(const Image<T>& img, const Matrix<bool>& SE, const Point<int>& SE_center)
        {
            Image er = erosion(img, SE, SE_center);
            return dilation(er, SE, SE_center);
        }

        static inline Image<T> closure(const Image<T>& img, const Matrix<bool>& SE, const Point<int>& SE_center)
        {
            Image dil = dilation(img, SE, SE_center);
            return erosion(dil, SE, SE_center);
        }


    };





}

#endif //DISPLAYIMAGE_IMAGE_H
