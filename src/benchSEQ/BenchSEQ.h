//
// Created by nagash on 23/10/16.
//

#ifndef STRUCTURED_BENCHSEQ_H
#define STRUCTURED_BENCHSEQ_H

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


class BenchSEQ : public convolutionBench::IConvBench<BenchSEQ> {
private:
// "structured-style" functions
    uchar *newImg(int rows, int cols, uchar initval = 0);

    uchar *cloneImg(const uchar *img, int rows, int cols);

    uchar *immerge(const uchar *img, int rows, int cols, int paddingTop, int paddingLeft, uchar initValue);

    void dilation(uchar *&img, int rows, int cols, const uchar *SE, int seRows, int seCols);

    void erosion(uchar *&img, int rows, int cols, const uchar *SE, int seRows, int seCols);

    void imshow(string message, uchar *img, int rows, int cols);

    void blockErosion(uchar *&img, int rows, int cols, const uchar *SE, int seRows, int seCols, int blockRows,
                      int blockCols);


private:


    const uchar *SE;
    uint seWidth;
    uint seHeight;


    int paddingTop; // todo: not floor??
    int paddingLeft;


    std::string imgPath;
    uchar *originalImg = nullptr;
    uchar *processingImg = nullptr;
    uint imgWidth;
    uint imgHeight;

    //uint nThreads = 0;
    bool useThreadsAsDivisor = false;


protected:


public:

    BenchSEQ() {}

    ~BenchSEQ() {
        if (originalImg != nullptr) delete[] originalImg;
        if (processingImg != nullptr) delete[] processingImg;
    }

    virtual void
    init(std::string imgPath, uint threads, uint se_width, uint se_height, bool useThreadsAsDivisor) override {
        seWidth = se_width;
        seHeight = se_height;
        SE = newImg(seHeight, seWidth, 1);

        this->imgPath = imgPath;
        cv::Mat imgCV = imread(imgPath, CV_LOAD_IMAGE_GRAYSCALE);
        imgWidth = imgCV.cols;
        imgHeight = imgCV.rows;
        if (originalImg != nullptr) delete[] originalImg;
        originalImg = cloneImg(imgCV.data, imgHeight,
                               imgWidth);// we need to clone because imgCV distructor will delete imgCV.data !

        //nThreads = 1;
        this->useThreadsAsDivisor = useThreadsAsDivisor;
    }


    virtual void showOriginalImg() {
        if (originalImg != nullptr)
            imshow("Original Image", originalImg, imgHeight, imgWidth);
    }

    virtual void showProcessedImg() override {
        if (originalImg != nullptr) {
            if (processingImg != nullptr)
                delete[] processingImg;
            processingImg = cloneImg(originalImg, imgHeight, imgWidth);
            run();
            imshow("Processed Image", processingImg, imgHeight, imgWidth);
        }
    }


    virtual const string getBenchName() const override {
        return "Bench STRUCT";
    }

    virtual const uint getSeWidth() const override {
        if (originalImg != nullptr) return seWidth;
        else return 0;
    }

    virtual const uint getSeHeight() const override {
        if (originalImg != nullptr) return seHeight;
        else return 0;
    }

    virtual const uint getImgWidth() const override {
        if (originalImg != nullptr) return imgWidth;
        else return 0;
    }

    virtual const uint getImgHeight() const override {
        if (originalImg != nullptr) return imgHeight;
        else return 0;
    }

    virtual const uint getThreads() const override {
        return 1;
    }

    virtual const std::string getImgPath() const override {
        return imgPath;
    }


    virtual void onPreRun() override {
        if (processingImg != nullptr) delete[] processingImg;
        processingImg = cloneImg(originalImg, imgHeight, imgWidth);
    }


// Curiously recurring template pattern: Static polymorphism
    void run() {
        erosion(processingImg, imgHeight, imgWidth, SE, seHeight, seWidth);
    }


    virtual void onPostRun() override {}

};


#endif //STRUCTURED_BENCHSEQ_H
