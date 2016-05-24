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

        Image(const Matrix<T>& matrix) : _mat(matrix) { } // deep copy of matrix
        Image(Matrix<T>&& matrix) : _mat(std::move(matrix)) { } // move constructor of matrix
        Image( std::string imgPath) : Image(readImageCV(imgPath)) {}


        // Copy, Assignmnet, Move
        Image( const Image& copy ) : _mat(copy._mat){}
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







        void immerge(int topPadding, int rightPadding, int bottomPadding, int leftPadding, T borderValue) {
            //this->_mat = _mat.makeImmersion(topPadding, rightPadding, bottomPadding, leftPadding, borderValue);
            this->_mat.immerge(topPadding, rightPadding, bottomPadding,leftPadding, borderValue);
        }
        Image<T> makeImmersion(int topPadding, int rightPadding, int bottomPadding, int leftPadding, T borderValue) {
            return image(_mat.makeImmersion(topPadding, rightPadding, bottomPadding, leftPadding, borderValue));;
        }




        inline void imshow(std::string msg = "") {
            cv::Mat m = _mat.toMatCV();
            cv::imshow(msg, m);
        }




        void dilation(const Matrix<bool>& SE, const Point<int>& SE_center)
        {
            //CV_Assert(bw_SE.depth() == CV_8U);  // accept only uchar Structuring Elements
            //CV_Assert(img.depth() == CV_8U);  // accept only uchar images



            Image copy = Image(*this);
            const int TOP_PADDING = SE_center.y();
            const int BOTTOM_PADDING = SE.rows() - SE_center.y();
            const int LEFT_PADDING = SE_center.x();
            const int RIGHT_PADDING = SE.cols() - SE_center.x();;

            copy.immerge(TOP_PADDING, RIGHT_PADDING, BOTTOM_PADDING, LEFT_PADDING, 255);
            //copy.imshow("dilation immersion"); //debug

            #pragma omp parallel for schedule(dynamic, 4)
            for (int y = 0; y < this->rows(); y++)
            {
                for (int x = 0; x < this->cols(); x++)
                {
                    uchar min = copy.get(y+TOP_PADDING, x+LEFT_PADDING);

                    for (int i = 0; i < SE.rows(); i++)
                    {
                        for (int j = 0; j < SE.cols(); j++)
                        {
                            //TODO: make function version without this check - optimization for rectangular SE
                            if (SE.get(i, j) == true)// if SE[i][j]==1, apply the mask:
                            {
                                //const uchar& current = copy.get(y + (i-SE_center.y()) + TOP_PADDING, x +  (j-SE_center.x()) + LEFT_PADDING);
                                const uchar& current = copy.get(y + i, x + j);
                                if (current < min)
                                    min = current;
                            }
                        }
                    }

                    this->set(y, x, min);
                }
            }

        }


        void erosion(const Matrix<bool>& SE, const Point<int>& SE_center)
        {
            //CV_Assert(bw_SE.depth() == CV_8U);  // accept only uchar Structuring Elements
            //CV_Assert(img.depth() == CV_8U);  // accept only uchar images

            long double sysTime1 = time(0);

            Image copy = Image(*this);
            const int TOP_PADDING = SE_center.y();
            const int BOTTOM_PADDING = SE.rows() - SE_center.y();
            const int LEFT_PADDING = SE_center.x();
            const int RIGHT_PADDING = SE.cols() - SE_center.x();;

            copy.immerge(TOP_PADDING, RIGHT_PADDING, BOTTOM_PADDING, LEFT_PADDING, 0);
            //copy.imshow("erosion immersion"); // debug

            #pragma omp parallel for num_threads(this->rows()/8)
            for (int y = 0; y < this->rows(); y++)
            {
                for (int x = 0; x < this->cols(); x++)
                {
                    uchar max = copy.get(y+TOP_PADDING, x+LEFT_PADDING);
                    for (int i = 0; i < SE.rows(); i++)
                    {
                        for (int j = 0; j < SE.cols(); j++)
                        {

                            //TODO: make function version without this check - optimization for rectangular SE
                            if (SE.get(i, j) == true)// if SE[i][j]==1, apply the mask:
                            {
                                //const uchar& current = copy.get(y + (i-SE_center.y()) + TOP_PADDING, x +  (j-SE_center.x()) + LEFT_PADDING);
                                const uchar& current = copy.get(y + i, x + j);
                                if (current > max)
                                    max = current;
                            }
                        }
                    }

                    this->set(y, x, max);
                }
            }

        }



        inline void opening(const Matrix<bool>& SE, const Point<int>& SE_center)
        {
            erosion(SE, SE_center);
            dilation(SE, SE_center);
//            erosion_old(SE, SE_center);
//            dilation_old(SE, SE_center);
            // with erosion/dilation old, on imgBig.jpg - NagashLaptop - core i5 @ 2.38ghz ---> 10/11 seconds
            // with erosion/dilation optimized (immersion), on imgBig.jpg - NagashLaptop - core i5 @ 2.38ghz ---> 6/7 seconds
        }

        inline void closure(const Matrix<bool>& SE, const Point<int>& SE_center)
        {
            dilation(SE, SE_center);
            erosion(SE, SE_center);
//            dilation_old(SE, SE_center);
//            erosion_old(SE, SE_center);
        }





        // shallow converter
        inline cv::Mat toMatCV() const  {  return this->_mat.toMatCV();  }





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



    };





}

#endif //DISPLAYIMAGE_IMAGE_H







































//
//void dilation_old(const Matrix<bool>& SE, const Point<int>& SE_center)
//{
//    //CV_Assert(bw_SE.depth() == CV_8U);  // accept only uchar Structuring Elements
//    //CV_Assert(img.depth() == CV_8U);  // accept only uchar images
//
//    long double sysTime1 = time(0);
//
//    Image copy = Image(*this);
//
//    for (int y = 0; y < this->_mat.rows(); y++)
//    {
//        for (int x = 0; x < this->_mat.cols(); x++)
//        {
//            uchar min = copy.get(y, x);
//
//            for (int i = 0; i < SE.rows(); i++)
//            {
//                for (int j = 0; j < SE.cols(); j++)
//                {
//                    if ((y - SE_center.y() + i) >= 0 && (x - SE_center.x() + j) >= 0 &&
//                        (y - SE_center.y() + i) < copy.rows() &&
//                        (x - SE_center.x() + j) < copy.cols())
//                    { // CHECK if SE is inside the image edges
//                        if (SE.get(i, j) == true)// if SE[i][j]==1, apply the mask:
//                        {
//                            const uchar &current = copy.get(y - SE_center.y() + i, x - SE_center.x() + j);
//                            if (current < min)
//                                min = current;
//                        }
//                    }
//                }
//            }
//
//            this->set(y, x, min);
//        }
//    }
//    long double sysTime2 = time(0);
//    std::cout << "\nDilation Profiling, elapsed: " << sysTime2-sysTime1 << std::endl;
//
//}
//
//inline void erosion_old(const Matrix<bool>& SE, const Point<int>& SE_center)
//{
//    //CV_Assert(bw_SE.depth() == CV_8U);  // accept only uchar Structuring Elements
//    //CV_Assert(img.depth() == CV_8U);  // accept only uchar images
//
//    long double sysTime1 = time(0);
//
//    Image copy = Image(*this);
//
//    for (int y = 0; y < copy.rows(); y++)
//    {
//        for (int x = 0; x < copy.cols(); x++)
//        {
//            uchar max = copy.get(y, x);
//            for (int i = 0; i < SE.rows(); i++)
//            {
//                for (int j = 0; j < SE.cols(); j++)
//                {
//
//                    if( (y - SE_center.y() + i) >= 0 && (x - SE_center.x() + j) >= 0 &&
//                        (y - SE_center.y() + i) < copy.rows() && (x - SE_center.x() + j) < copy.cols())
//                    { // CHECK if SE is inside the image edges
//
//                        if (SE.get(i, j) == true)// if SE[i][j]==1, apply the mask:
//                        {
//                            const uchar& current = copy.get(y - SE_center.y() + i, x - SE_center.x() + j);
//                            if (current > max)
//                                max = current;
//                        }
//                    }
//                }
//            }
//
//            this->set(y, x, max);
//        }
//    }
//    long double sysTime2 = time(0);
//    std::cout << "\nErosion Profiling, elapsed: " << sysTime2-sysTime1 << std::endl;
//
//}