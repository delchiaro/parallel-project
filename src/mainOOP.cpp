#include <iostream>




#include "convolutionBench/IConvBench.h"
#include "convolutionBench/BenchManager.h"
#include "benchOOP/BenchOOP.h"
#include "benchOOP/ErosionBenchOOP.h"


using namespace std;



using namespace convolutionBench;



int main(int argc, char** argv ) {
    BenchManager benchManager(argc, argv);

//    ClosureBenchOOP bench;
//    benchManager.start<ClosureBenchOOP>(bench);

    ErosionBenchOOP bench;
    benchManager.start<ErosionBenchOOP>(bench);

}



