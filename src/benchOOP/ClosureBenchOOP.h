//
// Created by nagash on 23/10/16.
//

#ifndef STRUCTURED_CLOSUREBENCHOOP_H
#define STRUCTURED_CLOSUREBENCHOOP_H


#include "BenchOOP.h"

class ClosureBenchOOP : public BenchOOP<ClosureBenchOOP>  {
public:
    void run() {
        BenchOOP::originalImage.dilation( BenchOOP::SE );
    }


};


#endif //STRUCTURED_CLOSUREBENCHOOP_H
