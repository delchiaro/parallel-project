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


        Image(cv::Mat openCV_image) : _mat( openCV_image.data, openCV_image.rows, openCV_image.cols ) { }
        // will make a deep copy of openCV_image.data.. overhead but safer.

        Image( std::string imgPath) : Image(_readImageCV(imgPath)) {}

        Image(Matrix<T> matrix) :_mat(matrix) { }
        // will make a deep copy of matrix... overhead but safer.

        Image(int rows, int cols, const T& initVal){
            this->mat = Matrix<T>(rows, cols, initVal);
        }

        // copy constructor
        Image( const Image& copy ) : _mat(copy._mat){ }
        Image<T>& operator= (const Image &im) {
            this->_mat = im._mat;
        }
        Image<T> clone() const { return Image(*this); }









        inline const int& rows() const { return _mat._rows; }
        inline const int& cols() const { return _mat._cols; }


        // ~ ~ GETTERS ~ ~
        //2D:
        inline const T& get (int row, int col) const { return _pmat[_index(row, col)]; }
        inline       T  getV(int row, int col) const { return _pmat[_index(row, col)]; }
        //1D:
        inline const T& get (int index) const { return _pmat[index]; }
        inline       T  getV(int index) const { return _pmat[index]; }
        //RAW:
        inline T* getRaw() const  { return _pmat; } // pointer to const data (immutable - read only)



        // ~ ~ SETTERS ~ ~
        //2D:
        inline void set(int row, int col, const T& value) { _pmat[_index(row, col)] = value; }
        //1D:
        inline void set(int index, const T& value) { _pmat[index] = value; }













        inline cv::Mat toMatCV() const  {  return this->_mat.toMatCV();  }
//        inline cv::Mat toMatCV_for() const
//        {
//            cv::Mat cvMat =  cv::Mat( cv::Size(this->rows(), this->cols()), 0);
//            for(int i = 0; i < this->rows(); i++)
//                for(int j = 0; j < this->cols(); j++)
//                    cvMat.row(i).col(j).data = this->get(i, j);
//            return cvMat;
//            // NB: We hope in Return Value Optimization (see wikipedia)
//        }














        void immerge(int topPadding, int rightPadding, int bottomPadding, int leftPadding, T borderValue)
        {
            this->_mat = this->_mat.immersion(topPadding, rightPadding, bottomPadding, leftPadding, borderValue);
        }
        Image<T> getImmersion(int topPadding, int rightPadding, int bottomPadding, int leftPadding, T borderValue)
        {
            return Image<T>(_mat.immersion(topPadding, rightPadding, bottomPadding, leftPadding, borderValue));
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

    protected:
        static cv::Mat _readImageCV(std::string path)
        {
            using namespace cv;
            Mat image =  imread( path, CV_LOAD_IMAGE_GRAYSCALE); //CV_LOAD_IMAGE_GRAYSCALE);

            if ( !image.data )
            {
                // printf("No image data \n");
                // TODO: generate exception
            }
            return imread( path, CV_LOAD_IMAGE_GRAYSCALE); //CV_LOAD_IMAGE_GRAYSCALE);
        }


    public:
        // TODO: remove this method.. deprecated! (use constructor instead)
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

            for (int y = 0; y < rows(); y++) {
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
