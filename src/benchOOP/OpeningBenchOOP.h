//
// Created by nagash on 23/10/16.
//

#ifndef STRUCTURED_OPENINGBENCHOOP_H
#define STRUCTURED_OPENINGBENCHOOP_H


#include "BenchOOP.h"

class OpeningBenchOOP : public BenchOOP<OpeningBenchOOP>  {
public:
    void run() {
        BenchOOP::processingImage.dilation( BenchOOP::SE);
    }
};


#endif //STRUCTURED_OPENINGBENCHOOP_H
