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
        cout << "NB: options between two rows are mutually exclusive (use max one of them) \n\n";

        cout << "         LONG               |    SHORT     -    DESCRIPTION                                      DEFAULT   \n";
        cout << " -----------------------------------------------------------------------------------------------------------------\n";
        cout << " bench        <NUM>         |  b  <NUM>    -   Specify the number NUM of bench to do for each config.     {100}    \n";
        cout << "                            |                  For each test will be computed avg time and variance               \n";
        cout << " -----------------------------------------------------------------------------------------------------------------\n";
        cout << " loop         <NUM>         |  l  <NUM>    -   Specify the number NUM of iterations/loops for each bench {1}      \n";
        cout << " -----------------------------------------------------------------------------------------------------------------\n";
        cout << " thread        <N>          |  t  <NUM>    -   Specify the number of thread to use                     {disabled} \n";
//      cout << " div-thread    <N>          |  dt <NUM>    -   Specify the number of threads as imgRows/NUM            {8}        \n";
        cout << "                            |                  Specify ft or dt multiple time to run multiple tests               \n";
        cout << " -----------------------------------------------------------------------------------------------------------------\n";
        cout << " se-wh      <NUM>           |  swh <W> <H> -   Set the width and height of Structuring Element           {7}      \n";
        cout << " se-dim     <NUM>           |  sd  <NUM>   -   Set same width and height for Structuring Element         {7}      \n";
        cout << "                            |                  Specify se-dim  or se-wh multiple time to run multiple tests       \n";
        cout << " -----------------------------------------------------------------------------------------------------------------\n";
        cout << " -----------------------------------------------------------------------------------------------------------------\n";
        cout << " preliminar-run <NUM>       |  pr          -   Set number of loops for preliminar-run (before each bench){5}  \n";
        cout << " -----------------------------------------------------------------------------------------------------------------\n";
        cout << " preliminar-global <NUM>    |  pg          -   Set number of loops for a preliminar for the first bench  {50}  \n";

//        cout << " -----------------------------------------------------------------------------------------------------------------\n";
//        cout << " verbose-preliminar         |  vp          -   Enable preliminar triple test output in files           {enabled}  \n";
        cout << " -----------------------------------------------------------------------------------------------------------------\n";
        cout << " -----------------------------------------------------------------------------------------------------------------\n";
        cout << " seconds-mult    <NUM>      | sm <NUM>     -   Multiply seconds for a NUMber (use 1000 for ms)            {1}   \n";
        cout << " -----------------------------------------------------------------------------------------------------------------\n";
        cout << " html-table-out    <PATH>   | hto <PATH>   -   Create new html-table, write results and close          {disabled} \n";
        cout << " html-table-new    <PATH>   | htn <PATH>   -   Create new html-table, write results, leave open        {disabled} \n";
        cout << " html-table-append <PATH>   | hta <PATH>   -   Append new results to an opened table                   {disabled} \n";
        cout << " html-table-close  <PATH>   | htc <PATH>   -   Append new results to an opened table and close         {disabled} \n";
        cout << " -----------------------------------------------------------------------------------------------------------------\n";
        cout << " csv-table-new    <PATH>    | ctn <PATH>   -   Create new csv-table, write results                     {disabled} \n";
        cout << " csv-table-append <PATH>    | cta <PATH>   -   Append new results to a csv table                       {disabled} \n";
        cout << " -----------------------------------------------------------------------------------------------------------------\n";
        cout << " output                     |  o           -   Show output in terminal                                 {disabled} \n";
        cout << " verbose                    |  v           -   Show verbose output in terminal                         {disabled} \n";
        cout << " -----------------------------------------------------------------------------------------------------------------\n";
        cout << " show-img                   |  s           -   Show output image of the first erosion                  {disabled} \n";
        cout << " -----------------------------------------------------------------------------------------------------------------\n";
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
            if (argcheck(argv[i], "bench", "b"))  benchNumber = atoi(argv[++i]);
            else if (argcheck(argv[i], "loop", "l"))  benchLoops = atoi(argv[++i]);

            else if (argcheck(argv[i], "preliminar-run", "pr"))   preliminar_run = atoi(argv[++i]);
            else if (argcheck(argv[i], "preliminar-global", "pg"))   preliminar_global = atoi(argv[++i]);

                //else if (argcheck(argv[i], "verbose-preliminar", "vp"))   preliminar_out = true;

            else if (argcheck(argv[i], "fixed-thread", "ft"))   fixedThreadList.push_back(atoi(argv[++i]));
            else if (argcheck(argv[i], "threads", "tr"))   fixedThreadList.push_back(atoi(argv[++i]));
            else if (argcheck(argv[i], "thread", "t"))   fixedThreadList.push_back(atoi(argv[++i]));

                //else if (argcheck(argv[i], "div-thread", "dt"))     divThreadList.push_back(atoi(argv[++i]));


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



            else if (argcheck(argv[i], "seconds-mult", "sm"))  secondsMult = atoi(argv[++i]);

            else if (argcheck(argv[i], "html-table-out", "hto")) {
                html_table_create = html_table_append= html_table_close = true;
                out_html = argv[++i];
            }
            else if (argcheck(argv[i], "html-table-new", "htn")) {
                html_table_create = true;
                out_html = argv[++i];
            }
            else if(argcheck(argv[i], "html-table-append", "hta")) {
                html_table_append = true;
                out_html = argv[++i];
            }
            else if(argcheck(argv[i], "html-table-close", "thc") ) {
                html_table_append = html_table_close = true;
                out_html = argv[++i];
            }

            else if(argcheck(argv[i], "csv-table-new", "ctn") ) {
                csv_table_create = csv_table_append = true;
                out_csv = argv[++i];
            }
            else if(argcheck(argv[i], "csv-table-append", "cta") ) {
                csv_table_append = true;
                out_csv = argv[++i];
            }

            else if(argcheck(argv[i], "output", "o") ) terminal = true;
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
        const char* mArg;
        if(arg[0] == '-' || arg[0] == '/') {
            if (arg[1] == '-')
                mArg = arg + 2;
            else mArg = arg + 1;
        }
        else mArg = arg;

        if(strcmp(mArg, short_param ) == 0 || strcmp(mArg, long_param ) == 0)
            return true;
        else return false;
    }



} // namespace convolutionBench;