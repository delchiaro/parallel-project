//
// Created by nagash on 21/10/16.
//

#include "BenchManager.h"
#include "../utils/TimeProfiler.h"
#include <fstream>
#include <iostream>

using namespace std;

namespace convolutionBench {

    BenchManager::BenchManager( int argc, char** argv) : table(ofstream()) {
        processParam(argc, argv);
    }


    void BenchManager::printParamList() {
    cout << "Test image erosion parallel performance with openMP.";
    cout << "\n";
    cout << "Usage:   command <INPUT_IMAGE> [options] \n\n";
    cout << "NB: options between two rows are mutually exclusive (use max one of them) \n\n";
    cout << "         LONG          |    SHORT     -    DESCRIPTION                                      DEFAULT   \n";
    cout << " ---------------------------------------------------------------------------------------------------  \n";
    cout << " iter         <NUM>    |  i  <NUM>    -   Specify the number NUM of test to do              {100}     \n";
    cout << " ---------------------------------------------------------------------------------------------------  \n";
    cout << " no-preliminar         |  np          -   Disable the preliminar triple test run            {enabled} \n";
    cout << " ---------------------------------------------------------------------------------------------------  \n";
    cout << " fixed-thread  <N>     |  ft <NUM>    -   Specify the number of thread to use               {disabled}\n";
    cout << " div-thread    <N>     |  dt <NUM>    -   Specify the number of threads as imgRows/NUM      {8}       \n";
    cout << "                       |                  Specify ft or dt multiple time to run multiple tests         \n";
    cout << " ---------------------------------------------------------------------------------------------------  \n";
    cout << " se-wh      <NUM>      |  swh <W> <H> -   Set the width and height of Structuring Element   {7}       \n";
    cout << " se-dim     <NUM>      |  sd  <NUM>   -   Set same width and height for Structuring Element {7}       \n";
    cout << "                       |                  Specify se-dim  or se-wh multiple time to run multiple tests\n";
    cout << " ---------------------------------------------------------------------------------------------------  \n";
    cout << " table-out    <PATH>   |  to <PATH>   -   Create new html-table, write results and close    {disabled}\n";
    cout << " table-new    <PATH>   |  tn <PATH>   -   Create new html-table, write results, leave open  {disabled}\n";
    cout << " table-append <PATH>   |  ta <PATH>   -   Append new results to an opened table             {disabled}\n";
    cout << " table-close  <PATH>   |  tc <PATH>   -   Append new results to an opened table and close   {disabled}\n";
    cout << " ---------------------------------------------------------------------------------------------------  \n";
    cout << " terminal              |  t           -   Show output in terminal                           {disabled}\n";
    cout << " verbose               |  v           -   Show verbose output in terminal                   {disabled}\n";
    cout << " ---------------------------------------------------------------------------------------------------  \n";
    cout << " show-img              |  s           -   Show output image of the first erosion            {disabled}\n";
    cout << " ---------------------------------------------------------------------------------------------------  \n";
    cout << "\n\n";
}


    void BenchManager::processParam(int argc, char** argv)
    {

        if(argc < 2) {
            printParamList();
            exit(0);
        }
        imgPath = argv[1];

        for(int i = 2; i < argc; i++)
        {
            if (argcheck(argv[i], "iter", "i"))  benchLoops = atoi(argv[++i]);
            else if (argcheck(argv[i], "no-preliminar", "np"))   preliminar_triple_run = false;

            else if (argcheck(argv[i], "fixed-thread", "ft"))   fixedThreadList.push_back(atoi(argv[++i]));
            else if (argcheck(argv[i], "div-thread", "dt"))     divThreadList.push_back(atoi(argv[++i]));


            else if (argcheck(argv[i], "se-dim", "sd")) {
                uint dim = atoi(argv[++i]);
                tuple<uint, uint> dims(dim, dim);
                seDimsList.push_back( dims );
            }
            else if (argcheck(argv[i], "se-wh", "swh")) {
                uint w = atoi(argv[++i]);
                uint h = atoi(argv[++i]);
                tuple<uint, uint> dims(w, h);
                seDimsList.push_back( dims );
            }



            else if (argcheck(argv[i], "table-out", "to")) {
                table_create = table_append= table_close = true;
                out = argv[++i];
            }
            else if (argcheck(argv[i], "table-new", "tn")) {
                table_create = true;
                out = argv[++i];
            }
            else if(argcheck(argv[i], "table-append", "ta")) {
                table_append = true;
                out = argv[++i];
            }
            else if(argcheck(argv[i], "table-close", "tc") ) {
                table_append = table_close = true;
                out = argv[++i];
            }


            else if(argcheck(argv[i], "terminal", "t") ) terminal = true;
            else if(argcheck(argv[i], "verbose",  "v") ) terminal = verbose = true;
            else if(argcheck(argv[i], "show-img", "s") ) showImg = true;




            else if(argcheck(argv[i], "help", "h")
                    || argcheck(argv[i], "-help", "-h")
                    || argcheck(argv[i], "--help", "--h")
                    || argcheck(argv[i], "-?", "?"))
            {
                printParamList();
                exit(0);
            }

            else {
                cout << "INVALID PARAMETER '" << argv[i] << "' AT POSITION: " << i-1 << std::endl << std::endl;
                printParamList();
                exit(1);
            }

        }
    }





    bool BenchManager::argcheck(const char* arg, const char* long_param, const char* short_param) {
    if(strcmp(arg, short_param ) == 0 || strcmp(arg, long_param ) == 0)
        return true;
    else return false;
}



} // namespace convolutionBench;