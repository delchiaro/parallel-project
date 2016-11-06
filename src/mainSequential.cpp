//
// Created by Franco Yang on 23/05/16.
//


#include "utils/TimeProfiler.h"
#include "benchSEQ/BenchSEQ.h"
#include "convolutionBench/BenchManager.h"
#include "benchSTRUCT/BenchSTRUCT.h"



using namespace convolutionBench;

int main(int argc, char** argv){
    BenchManager benchManager(argc, argv);

    BenchSEQ bench;
    benchManager.start<BenchSEQ>(bench);
}