//
// Created by nagash on 21/10/16.
//

#ifndef STRUCTURED_BENCHOOP_H
#define STRUCTURED_BENCHOOP_H

#include "../utils/utils.h"
#include "../convolutionBench/IConvBench.h"
#include "../imProc/Image.h"
#include "../imProc/StructuringElement.h"


using namespace imProc;


// Curiously recurring template pattern
template <class T>
class BenchOOP : public convolutionBench::IConvBench<T>
{
private:
    bool imgLoaded = false;
    bool imgProcessed = false;
    uint nThreads = 0;
    std::string imgPath;



protected:
    StructuringElement SE = StructuringElement(1,1);
    Image originalImage = Image(1,1,0);
    Image benchImage = Image(1,1,0);


public:

    BenchOOP() {}

    virtual void init(std::string imgPath, uint threads, uint se_width, uint se_height, bool useThreadsAsDivisor) override {
        SE = StructuringElement(se_width, se_height);

        originalImage = Image(imgPath);
        if(useThreadsAsDivisor)
            originalImage.setThreads(threads / originalImage.rows());
        else originalImage.setThreads(threads);
        this->nThreads = originalImage.getThreads();
        benchImage = Image(originalImage);
        this->imgPath = imgPath;

        imgLoaded = true;
    }

    virtual void showOriginalImg()  {
        if(imgLoaded) originalImage.imshow("Original Image");
    };
    virtual void showProcessedImg() override {
        if(imgLoaded)
        {
            // enabling this check, we will show the image after ALL the processing.
            // disabling this check, we always make a new processing af a copy of the original image before showing
            // if(imgProcessed == false)
            {
                benchImage = Image(originalImage);
                static_cast<T*>(this)->run();
            }
            benchImage.imshow("Processed Image");
        }
    }

    virtual  void onPreRun() override {
        benchImage = Image(originalImage);
    }
    virtual void onPostRun() override {
        imgProcessed = true;
    }

    virtual const std::string getBenchName() const override{
        return "Bench OOP";
    }

    virtual const uint getSeWidth() const override {
        return SE.cols();
    }
    virtual const uint getSeHeight() const override {
        return SE.rows();
    }
    virtual const uint getImgWidth() const override {
        if(imgLoaded)
            return originalImage.cols();
        else return 0;
    }
    virtual const uint getImgHeight() const override {
        if(imgLoaded)
            return originalImage.rows();
        else return 0;
    }

    virtual const uint getThreads() const override {
        return nThreads;
    }
    virtual const std::string getImgPath() const override {
        return imgPath;
    }
    // Curiously recurring template pattern: Static polymorphism
    // Removed here, added another level of abstaction
    // (run is specified in the subclass of BenchOOP)
//    void run() {
//        originalImage.erosion(SE);
//       // static_cast<T*>(this)->_run();
//    }

};


#endif //STRUCTURED_BENCHOOP_H









// Deprecated: I was using an enumeration to select the convolutionBench function, and a function reference to run the right function.
// problems: function reference must be specified in the constructor, it was a mess.
// possible solutions:
//      - Virtual functions (initial aproach, bad: in run we have a late binding, i.e. switch() + function pointer, i.e. switch() + derefereniation)
//      - Function pointer (bad: in run we have a derefereniation to do each time)
//      - Static Polimorfism with Curiously recurring template pattern: no overhead added in run function!


//  enum BenchFunction { opening, closure, erosion, dilation };
//

//
//    uint rows = 0;
//    uint cols = 0;
//    virtual uint getImageRows() override { return rows; }
//    virtual uint getImageCols() override { return cols; }

//class ProcessingFunc {
//public:
//
//    void (&processingFunction)(const StructuringElement& se); // function reference
//
//    // set function reference
//    ProcessingFunc(void (&processingFunction)(const StructuringElement& se))
//            : processingFunction(processingFunction) {}
//};
