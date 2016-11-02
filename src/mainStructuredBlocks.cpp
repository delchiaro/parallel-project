//
// Created by Franco Yang on 23/05/16.
//


#include "utils/TimeProfiler.h"
#include "benchOOP/ClosureBenchOOP.h"
#include "convolutionBench/BenchManager.h"
#include "benchBLOCK/BenchBLOCK.h"



using namespace convolutionBench;

int main(int argc, char** argv){
    BenchManager benchManager(argc, argv);

    BenchBLOCK bench;
    benchManager.start<BenchBLOCK>(bench);
}