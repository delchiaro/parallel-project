//
// Created by nagash on 23/10/16.
//

#ifndef STRUCTURED_DILATIONBENCHOOP_H
#define STRUCTURED_DILATIONBENCHOOP_H


#include "BenchOOP.h"

class DilationBenchOOP : public BenchOOP<DilationBenchOOP> {
public:
    void run() {
        BenchOOP::originalImage.dilation( BenchOOP::SE);
    }

};


#endif //STRUCTURED_DILATIONBENCHOOP_H
