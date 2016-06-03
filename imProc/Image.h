//
// Created by Nagash on 09/05/16.
//

#ifndef DISPLAYIMAGE_IMAGE_H
#define DISPLAYIMAGE_IMAGE_H


#include "imProc.h"

#include "Matrix.h"
#include "Point.h"
#include <opencv2/opencv.hpp>
#include <iostream>

#include <omp.h>


#define RECTANGLE_KERNEL_OPTIMIZATION

namespace imProc
{

    template <typename T> class Image {
    private:
        Matrix<T> _mat;
    protected:

    public:

        Image(int rows, int cols, const T& initVal) : _mat(rows, cols, initVal){
        }

        Image(const Matrix<T>& matrix) : _mat(matrix) { } // deep copy of matrix
        Image(Matrix<T>&& matrix) : _mat(std::move(matrix)) { } // move constructor of matrix
        Image( std::string imgPath) : Image(readImageCV(imgPath)) {}


        // Copy, Assignmnet, Move
        __forceinline inline Image( const Image& copy ) : _mat(copy._mat){}
        __forceinline inline Image<T>& operator= (const Image &im) {
            _mat = im._mat;
        }
        __forceinline inline Image(Image&& move ) :  _mat(std::move(move._mat)) {
        }
        __forceinline inline Image<T>& operator= (Image&& move) {
            _mat = std::move(move._mat);
        }








        __forceinline inline const int&   rows()  const { return _mat.rows(); }
        __forceinline inline const int&   cols()  const { return _mat.cols(); }


        // ~ ~ GETTERS ~ ~
        __forceinline inline const T&     get (int row, int col)  const { return _mat.get(row, col);   }
        __forceinline inline const T&     get (int index)         const { return _mat.get(index);      }
        __forceinline inline       T      getV(int row, int col)  const { return _mat.get(row, col);   }
        __forceinline inline       T      getV(int index)         const { return _mat.get(index);      }
        __forceinline inline Matrix<T>    getMatrix()             const { return _mat;                 }


        // ~ ~ SETTERS ~ ~
        __forceinline inline void         set(int row, int col, const T& value)   { _mat.set(row, col, value); }
        __forceinline inline void         set(int index, const T& value)          { _mat.set(index, value); }







        __forceinline inline void immerge(int topPadding, int rightPadding, int bottomPadding, int leftPadding, T borderValue) {
            //this->_mat = _mat.makeImmersion(topPadding, rightPadding, bottomPadding, leftPadding, borderValue);
           _mat.immerge(topPadding, rightPadding, bottomPadding,leftPadding, borderValue);
        }
        __forceinline inline Image<T> makeImmersion(int topPadding, int rightPadding, int bottomPadding, int leftPadding, T borderValue) {
            return Image(_mat.makeImmersion(topPadding, rightPadding, bottomPadding, leftPadding, borderValue));;
        }




        inline void imshow(std::string msg = "") {
            cv::Mat m = _mat.toMatCV();
            cv::imshow(msg, m);
        }




        void erosion(const Matrix<bool>& SE, const Point<int>& SE_center)
        {
            //CV_Assert(bw_SE.depth() == CV_8U);  // accept only uchar Structuring Elements
            //CV_Assert(img.depth() == CV_8U);  // accept only uchar images

            const int TOP_PADDING = SE_center.y();
            const int BOTTOM_PADDING = SE.rows() - SE_center.y();
            const int LEFT_PADDING = SE_center.x();
            const int RIGHT_PADDING = SE.cols() - SE_center.x();;

            Image immersion = makeImmersion(TOP_PADDING, RIGHT_PADDING, BOTTOM_PADDING, LEFT_PADDING, 255);
            //immersion.imshow("dilation immersion"); //debug

            // num_threads(20)
            // #pragma omp parallel for schedule(dynamic, 4)
            #pragma omp parallel for num_threads(this->rows()/8)
            for (int y = 0; y < rows(); y++)
            {
                int y_TOP_PADD = y+TOP_PADDING;//optimization

                for (int x = 0; x < cols(); x++)
                {
                    uchar min = immersion.get(y_TOP_PADD, x+LEFT_PADDING); // optimization //uchar max = copy.get(y + TOP_PADDING, x+LEFT_PADDING);

                    for (int i = 0; i < SE.rows(); i++)
                    {
                        int yi = y + i;
                        for (int j = 0; j < SE.cols(); j++)
                        {

                            #ifndef RECTANGLE_KERNEL_OPTIMIZATION
                            if (SE.get(i, j) == true)// if SE[i][j]==1, apply the mask:
                            {
                            #endif
                                //const uchar& current = copy.get(y + (i-SE_center.y()) + TOP_PADDING, x +  (j-SE_center.x()) + LEFT_PADDING);
                                const uchar current = immersion.get(yi, x + j);
                                if (current < min)
                                    min = current;
                            #ifndef RECTANGLE_KERNEL_OPTIMIZATION
                            }
                            #endif
                        }
                    }

                    set(y, x, min);
                }
            }

        }


        void dilation(const Matrix<bool>& SE, const Point<int>& SE_center)
        {
            //CV_Assert(bw_SE.depth() == CV_8U);  // accept only uchar Structuring Elements
            //CV_Assert(img.depth() == CV_8U);  // accept only uchar images

            long double sysTime1 = time(0);

            Image copy = Image(*this);
            const int TOP_PADDING = SE_center.y();
            const int BOTTOM_PADDING = SE.rows() - SE_center.y();
            const int LEFT_PADDING = SE_center.x();
            const int RIGHT_PADDING = SE.cols() - SE_center.x();;

            Image immersion = makeImmersion(TOP_PADDING, RIGHT_PADDING, BOTTOM_PADDING, LEFT_PADDING, 255);
            //immersion.imshow("erosion immersion"); // debug


            //num_threads(20)num_threads(this->rows()/8)
            // #pragma omp parallel for num_threads(this->rows()/8)
            #pragma omp parallel for  num_threads(this->rows()/8)
            for (int y = 0; y < rows(); y++)
            {
                int y_TOP_PADD = y+TOP_PADDING;//optimization
                for (int x = 0; x < cols(); x++)
                {
                    uchar max = immersion.get(y_TOP_PADD, x+LEFT_PADDING); // optimization //uchar max = copy.get(y + TOP_PADDING, x+LEFT_PADDING);

                    for (int i = 0; i < SE.rows(); i++)
                    {
                        int yi = y+i; // optimization

                        for (int j = 0; j < SE.cols(); j++)
                        {

                            #ifndef RECTANGLE_KERNEL_OPTIMIZATION
                            if (SE.get(i, j) == true)// if SE[i][j]==1, apply the mask:
                            {
                            #endif
                                //const uchar& current = copy.get(y + (i-SE_center.y()) + TOP_PADDING, x +  (j-SE_center.x()) + LEFT_PADDING);
                                const uchar current = immersion.get(yi, x + j); //optimization
                                if (current > max)
                                    max = current;
                            #ifndef RECTANGLE_KERNEL_OPTIMIZATION
                            }
                            #endif
                        }
                    }

                    set(y, x, max);
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
        inline cv::Mat toMatCV() const  {  return _mat.toMatCV();  }





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