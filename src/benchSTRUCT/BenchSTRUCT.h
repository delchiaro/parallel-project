//
// Created by nagash on 23/10/16.
//

#ifndef STRUCTURED_BENCHSTRUCT_H
#define STRUCTURED_BENCHSTRUCT_H

#include "../utils/utils.h"
#include "../convolutionBench/IConvBench.h"
#include <iostream>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>


using namespace std;
using namespace cv;

#define INDEX(row, col, cols) ((col) + (row) * (cols))

#define RECTANGLE_KERNEL_OPTIMIZATION


class BenchSTRUCT : public convolutionBench::IConvBench<BenchSTRUCT>
{
private:
    // "structured-style" functions
    uchar* newImg(int rows, int cols, uchar initval = 0);
    uchar* cloneImg(const uchar* img, int rows, int cols);
    uchar* immerge(const uchar* img, int rows, int cols, int paddingTop , int paddingLeft , uchar initValue);
    void dilation(uchar*& img, int rows, int cols, const uchar* SE, int seRows, int seCols);
    void erosion(uchar*& img, int rows, int cols, const uchar* SE, int seRows, int seCols);
    void imshow(string message, uchar* img, int rows, int cols);
    void blockErosion(uchar*& img, int rows, int cols, const uchar* SE, int seRows, int seCols, int blockRows, int blockCols);



private:


    const uchar* SE;
    uint seWidth;
    uint seHeight;



    std::string imgPath;
    Mat imgCV;
    uchar* img = nullptr;
    uint imgWidth;
    uint imgHeight;
    uchar* imgProcessing = nullptr;

    uint nThreads = 0;
    bool useThreadsAsDivisor = false;



protected:


public:

    BenchSTRUCT() {}

    ~BenchSTRUCT() {
//        if(img != nullptr) // imgCV contains this img pointer, and will manage the memory for us! No delete img or we will get a runtime error.
//            delete img;
        if(imgProcessing != nullptr)
            delete imgProcessing;
    }

    virtual void init(std::string imgPath, uint threads, uint se_width, uint se_height, bool useThreadsAsDivisor) override {
        seWidth = se_width;
        seHeight = se_height;
        SE =  newImg(seHeight, seWidth, 1);

        this->imgPath = imgPath;
        imgCV = imread(imgPath, CV_LOAD_IMAGE_GRAYSCALE);
        imgWidth = imgCV.cols;
        imgHeight = imgCV.rows;
        img = imgCV.data;

        nThreads = threads;
        this->useThreadsAsDivisor = useThreadsAsDivisor;
    }


    virtual void showOriginalImg()  {
        if(img != nullptr)
             imshow("Original Image", img, imgHeight, imgWidth);
    }
    virtual void showProcessedImg() override {
        if(img != nullptr)
        {
            if(imgProcessing != nullptr)
                delete[] imgProcessing;
            imgProcessing = cloneImg(img, imgHeight, imgWidth);
            run();
            imshow("Processed Image", imgProcessing, imgHeight, imgWidth);
        }
    }

    virtual  void onPreRun() override {
        if(imgProcessing != nullptr) delete imgProcessing;
        imgProcessing = cloneImg(img, imgHeight, imgWidth);
    }
    virtual void onPostRun() override {}


    virtual const uint getSeWidth() const override {
        if(img != nullptr) return seWidth;
        else return 0;
    }
    virtual const uint getSeHeight() const override {
        if(img != nullptr) return seHeight;
        else return 0;
    }
    virtual const uint getImgWidth() const override {
        if(img != nullptr) return imgWidth;
        else return 0;
    }
    virtual const uint getImgHeight() const override {
        if(img != nullptr) return imgHeight;
        else return 0;
    }

    virtual const uint getThreads() const override {
        return nThreads;
    }
    virtual const std::string getImgPath() const override {
        return imgPath;
    }


    // Curiously recurring template pattern: Static polymorphism
    void run() {
        erosion(imgProcessing, imgHeight, imgWidth, SE, seHeight, seWidth);
    }

};


#endif //STRUCTURED_BENCHSTRUCT_H
