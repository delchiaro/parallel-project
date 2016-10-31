//
// Created by nagash on 23/10/16.
//

#ifndef STRUCTURED_EROSIONBENCHOOP_H
#define STRUCTURED_EROSIONBENCHOOP_H


#include "BenchOOP.h"

class ErosionBenchOOP : public BenchOOP<ErosionBenchOOP> {
public:

    void run() {
        BenchOOP::processingImage.erosion( BenchOOP::SE);
    }

};


#endif //STRUCTURED_EROSIONBENCHOOP_H
