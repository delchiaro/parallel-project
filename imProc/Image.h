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


    template <typename T> class Image {
    private:
        Matrix<T> _mat;
    protected:

    public:

        Image(int rows, int cols, const T& initVal){
            this->mat = Matrix<T>(rows, cols, initVal);
        }

        Image(const Matrix<T>& matrix) :_mat(matrix) { } // deep copy of matrix
        Image(Matrix<T>&& matrix) :_mat(std::move(matrix)) { } // move constructor of matrix

        Image( std::string imgPath) : Image(readImageCV(imgPath)) {}


        // Copy, Assignmnet, Move
        Image( const Image& copy ) : _mat(copy._mat){ }
        Image<T>& operator= (const Image &im) {
            this->_mat = im._mat;
        }
        Image(Image&& move ) :  _mat(std::move(move._mat)) {
        }
        Image<T>& operator= (Image&& move) {
            this->_mat = std::move(move._mat);
        }








        inline const int&   rows()  const { return _mat.rows(); }
        inline const int&   cols()  const { return _mat.cols(); }


        // ~ ~ GETTERS ~ ~
        inline const T&     get (int row, int col)  const { return _mat.get(row, col);   }
        inline const T&     get (int index)         const { return _mat.get(index);      }
        inline       T      getV(int row, int col)  const { return _mat.get(row, col);   }
        inline       T      getV(int index)         const { return _mat.get(index);      }
        inline Matrix<T>    getMatrix()             const { return _mat;                 }


        // ~ ~ SETTERS ~ ~
        inline void         set(int row, int col, const T& value)   { _mat.set(row, col, value); }
        inline void         set(int index, const T& value)          { _mat.set(index, value); }






        // shallow converter
        inline cv::Mat toMatCV() const  {  return this->_mat.toMatCV();  }



        void immerge(int topPadding, int rightPadding, int bottomPadding, int leftPadding, T borderValue) {
            this->_mat = _mat.makeImmersion(topPadding, rightPadding, bottomPadding, leftPadding, borderValue);
        }
        Image<T> makeImmersion(int topPadding, int rightPadding, int bottomPadding, int leftPadding, T borderValue) {
            return Image(_mat.makeImmersion(topPadding, rightPadding, bottomPadding, leftPadding, borderValue));
        }




        inline void imshow(std::string msg = "") {
            cv::Mat m = _mat.toMatCV();
            cv::imshow(msg, m);
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


    public:
        static Image<T> readImageCV(std::string path)
        {
            cv::Mat cvImg =  cv::imread( path, CV_LOAD_IMAGE_GRAYSCALE); //CV_LOAD_IMAGE_GRAYSCALE);
            using namespace cv;
            if ( !cvImg.data )
            {
                // printf("No image data \n");
                // TODO: generate exception
            }
            Image<T> im = Image<T>( Matrix<T>(cvImg.data, cvImg.rows, cvImg.cols) );
            return im;

        }



        static Image<T> dilation(const Image<T>& img, const Matrix<bool>& SE, const Point<int>& SE_center)
        {
            //CV_Assert(bw_SE.depth() == CV_8U);  // accept only uchar Structuring Elements
            //CV_Assert(img.depth() == CV_8U);  // accept only uchar images

            Image output = Image(img);

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

            Image output = Image(img);

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
