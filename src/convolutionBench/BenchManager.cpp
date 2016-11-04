//
// Created by nagash on 21/10/16.
//

#include "BenchManager.h"
#include "../utils/TimeProfiler.h"
#include <fstream>
#include <iostream>

using namespace std;

namespace convolutionBench {

    BenchManager::BenchManager( int argc, char** argv)  {
        processParam(argc, argv);
    }


    void BenchManager::printParamList() {
        cout << "Test image erosion parallel performance with openMP.";
        cout << "\n";
        cout << "Usage:   command <INPUT_IMAGE> [options] \n\n";

        cout << "NB: each option can be specified with: 'option', '-option', '--option', '/option'\n";
        cout << "                                                                                                               \n";
        cout << "         LONG         | SHORT NAME    -    DESCRIPTION                                      DEFAULT   \n";
        cout << " --------------------------------------------------------------------------------------------------------------\n";
        cout << " loop           <NUM> |  l            -   Specify the number NUM of iterations/loops for each bench    {1}     \n";
        cout << " bench          <NUM> |  b            -   Specify the number NUM of bench to do for each single config {10}    \n";
        cout << "                      |                   For each test will be computed avg time and variance                 \n";
        cout << " --------------------------------------------------------------------------------------------------------------\n";
        cout << " thread         <NUM> |  t            -   Specify the number of thread to use                          {8}     \n";
        cout << "                      |                   Specify ft or dt multiple time to run multiple tests                 \n";
        cout << " --------------------------------------------------------------------------------------------------------------\n";
        cout << " se-wh        <W> <H> |  swh          -   Set the width and height of Structuring Element              {7}     \n";
        cout << " se-dim         <NUM> |  se           -   Set same width and height for Structuring Element            {7}     \n";
        cout << "                      |                   Specify multiple time to run multiple tests                          \n";
        cout << " --------------------------------------------------------------------------------------------------------------\n";
        cout << " pre-run        <NUM> |  pr           -   Set number of loops for preliminar-run (before each bench)   {0}     \n";
        cout << " pre-run-global <NUM> |  prg          -   Set number of loops for a preliminar for the first bench     {0}     \n";
        cout << " --------------------------------------------------------------------------------------------------------------\n";
        cout << "                                                                                                               \n";
        cout << " --------------------------------O U T P U T-------------------------------------------------------------------\n";
        cout << " Select max 1 of these:                                                                                        \n";
        cout << " csv-new       <PATH> | csv-n ; csv  -   Create new csv-table for the results                  (NB: OVERWRITE!)\n";
        cout << " csv-append    <PATH> | csv-a        -   Append results to a csv table                                         \n";
        cout << "                                                                                                               \n";
        cout << " Select max 1 of these:                                                                                        \n";
        cout << " html-out      <PATH> | htm-o ; htm  -   Create new html-table, write results and close tags   (NB: OVERWRITE!)\n";
        cout << " html-new      <PATH> | htm-n        -   Create new html-table, write results, leave open tags (NB: OVERWRITE!)\n";
        cout << " html-append   <PATH> | htm-a        -   Append results to an opened table                                     \n";
        cout << " html-close    <PATH> | htm-c        -   Append results to an opened table and close all html tags             \n";
        cout << "                                                                                                               \n";
        cout << " variance             | var          -   Use variance instead of std deviation in the table         {disabled} \n";
        cout << " print-all            | pa           -   Print all the single bench timing in the table             {disabled} \n";
        cout << " seconds-mult   <NUM> | sec-mul ; sm -   Multiply seconds for a Number (use 1000 for ms)               {1}     \n";
        cout << " --------------------------------------------------------------------------------------------------------------\n";
        cout << " output               |  o           -   Show output in terminal                                    {disabled} \n";
        cout << " verbose              |  v           -   Show verbose output in terminal                            {disabled} \n";
        cout << " show-img             |  s           -   Show the processed image (after a single execution)        {disabled} \n";
        cout << " --------------------------------------------------------------------------------------------------------------\n";
        cout << "\n\n";


//        cout << " -----------------------------------------------------------------------------------------------------------------\n";
//        cout << " verbose-preliminar         |  vp          -   Enable preliminar triple test output in files           {enabled}  \n";
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
            if      ( argcheck(argv[i], "bench",          "b"  ) )  benchNumber = atoi(argv[++i]);
            else if ( argcheck(argv[i], "loop",           "l"  ) )  benchLoops = atoi(argv[++i]);
            else if ( argcheck(argv[i], "pre-run",        "pr" ) )  preliminar_run = atoi(argv[++i]);
            else if ( argcheck(argv[i], "pre-run-global", "prg") )  preliminar_global = atoi(argv[++i]);
            else if ( argcheck(argv[i], "thread",         "t"  ) )  fixedThreadList.push_back(atoi(argv[++i]));
            else if (argcheck(argv[i],   "se-wh",         "swh") ) {
                uint w = atoi(argv[++i]);
                uint h = atoi(argv[++i]);
                tuple<uint, uint> dims(w, h);
                seDimsList.push_back( dims );
            }
            else if ( argcheck(argv[i], "se-dim",         "se" ) ) {
                uint dim = atoi(argv[++i]);
                tuple<uint, uint> dims(dim, dim);
                seDimsList.push_back( dims );
            }

            // TABLE OUT:
            else if ( argcheck(argv[i], "html-out",   "htm-o", "htm") ) {
                html_table_create = html_table_append = html_table_close = true;
                out_html = argv[++i];
            }
            else if ( argcheck(argv[i], "html-new",   "htm-n") ) {
                html_table_create = true;
                out_html = argv[++i];
            }
            else if( argcheck(argv[i], "html-append", "htm-a"))  {
                html_table_append = true;
                out_html = argv[++i];
            }
            else if( argcheck(argv[i], "html-close",  "htm-c") ) {
                html_table_append = html_table_close = true;
                out_html = argv[++i];
            }
            else if( argcheck(argv[i], "csv-new",     "csv-n", "csv") ) {
                csv_table_create = csv_table_append = true;
                out_csv = argv[++i];
            }
            else if( argcheck(argv[i], "csv-append",  "csv-a") ) {
                csv_table_append = true;
                out_csv = argv[++i];
            }

            // OUT FLAGS:
            else if( argcheck(argv[i], "seconds-mult", "sec-mul", "sm"))  secondsMult = atoi(argv[++i]);
            else if( argcheck(argv[i], "variance",     "var" ) ) useVariance = true;
            else if( argcheck(argv[i], "print-all",    "pa"  ) ) printAllBenchs = true;
            else if( argcheck(argv[i], "output",       "o"   ) ) terminal = true;
            else if( argcheck(argv[i], "verbose",      "v"   ) ) terminal = verbose = true;
            else if( argcheck(argv[i], "show-img",     "s"   ) ) showImg = true;


            else if(   argcheck(argv[i], "help", "h", "?" ) ) {
                printParamList();
                exit(0);
            }
            else {
                printParamList();
                cout << "UNKNOWN PARAMETER '" << argv[i] << "' AT POSITION: " << i-1 << std::endl << std::endl;
                exit(1);
            }

        }
    }




    bool BenchManager::argcheck(const char* arg, const char* long_param, const char* short_param, const char* short_param2) {
        const char* mArg;
        if(arg[0] == '-' || arg[0] == '/') {
            if (arg[1] == '-')
                mArg = arg + 2;
            else mArg = arg + 1;
        }
        else mArg = arg;

        if(strcmp(mArg, long_param ) == 0 || strcmp(mArg, short_param ) == 0 || strcmp(mArg, short_param2 ) == 0)
            return true;
        else return false;
    }
    bool BenchManager::argcheck(const char* arg, const char* long_param, const char* short_param) {
        return argcheck(arg, long_param, short_param, short_param);
    }



} // namespace convolutionBench;