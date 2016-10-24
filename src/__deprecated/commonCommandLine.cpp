////
//// Created by nagash on 21/10/16.
////
//
//#include <iostream>
//#include <opencv2/opencv.hpp>
//#include <fstream>
//#include <list>
//#include <tuple>
//
//#include "utils/TimeProfiler.h"
//
//#include "imProc/Point.h"
//#include "imProc/Matrix.h"
//#include "imProc/Image.h"
//
//
//
//using namespace std;
//
//typedef imProc::Image<uchar> Image;
//
//void printParamList();
//void processParam(int argc, char** argv);
//bool argcheck(const char* arg, const char* long_param, const char* short_param);
//void run(int fixed_thread, int div_thread, uint se_width, uint se_height);
//
//
//
//
//
//
//
//void mainCommandLine(int argc, char** argv ) {
//
//
//    processParam(argc, argv);
//
//
//    if(table_create)
//    {
//        ofstream table(out, ios_base::trunc );
//        table << "<table border=\"1px solid black\" style=\"border: 1px solid black; border-collapse: collapse\">\n\n";
//
//        table << "\t<tr>\n";
//        table << "\t\t<th>SE w</th>\n";
//        table << "\t\t<th>SE h</th>\n";
//        table << "\t\t<th>DivThr</th>\n";
//        table << "\t\t<th>FixThr</th>\n";
//        if(preliminar_triple_run)
//        {
//            table << "\t\t<th>Run 1</th>\n";
//            table << "\t\t<th>Run 2</th>\n";
//            table << "\t\t<th>Run 3</th>\n";
//        }
//        table << "\t\t<th>Loop x" << niter << "</th>\n";
//        table << "\t</tr>\n\n";
//        table.close();
//    }
//
//
//    if( seDimsList.empty() )
//        seDimsList.push_back( tuple<uint, uint>(DEFAULT_SE_DIM, DEFAULT_SE_DIM)  );
//
//    if( divThreadList.empty() && fixedThreadList.empty() )
//        divThreadList.push_back( DEFAULT_DIV_THREAD );
//
//
//
//    for (int sed = 0; sed < seDimsList.size(); sed++)
//    {
//        tuple<uint, uint> dims = seDimsList.front();
//        uint se_w = get<0>(dims);
//        uint se_h = get<0>(dims);
//        ofstream table;
//
//        while(divThreadList.empty() == false)
//        {
//            uint divThread = divThreadList.front();
//            divThreadList.pop_front();
//
//            if(table_append) {
//                table = ofstream(out, ios_base::app);
//                table << "\t<tr>\n";
//                //table << "\t\t<td colspan=\"" << divThreadList.size() + fixedThreadList.size() << "\">" << se_w << "</td>\n";
//                //table << "\t\t<td colspan=\"" << divThreadList.size() + fixedThreadList.size()  << "\">" << se_w << "</td>\n";
//                table << "\t\t<td>" << se_w << "</td>\n";
//                table << "\t\t<td>" << se_w << "</td>\n";
//                table << "\t\t<td>" << divThread << "</td>\n";
//                table << "\t\t<td></td>\n";
//
//                table.close();
//            }
//
//            run(0, divThread, get<0>(dims), get<1>(dims) );
//
//            if(table_append)  {
//                table = ofstream(out, ios_base::app);
//                table << "\t<tr>\n";
//                table.close();
//            }
//        }
//
//        while(fixedThreadList.empty() == false)
//        {
//            uint fixedThread = fixedThreadList.front();
//            fixedThreadList.pop_front();
//
//            if(table_append) {
//                table = ofstream(out, ios_base::app);
//                table << "\t<tr>\n";
//                //table << "\t\t<td colspan=\"" << divThreadList.size() + fixedThreadList.size() << "\">" << se_w << "</td>\n";
//                //table << "\t\t<td colspan=\"" << divThreadList.size() + fixedThreadList.size()  << "\">" << se_w << "</td>\n";
//                table << "\t\t<td>" << se_w << "</td>\n";
//                table << "\t\t<td>" << se_w << "</td>\n";
//                table << "\t\t<td></td>\n";
//                table << "\t\t<td>" << fixedThread << "</td>\n";
//                table.close();
//            }
//
//            run(fixedThread, 0, get<0>(dims), get<1>(dims) );
//
//
//            if(table_append)  {
//                table = ofstream(out, ios_base::app);
//                table << "\t<tr>\n";
//                table.close();
//            }
//        }
//
//        if(table_append) table << "\t</tr>\n";
//    }
//
//    if(table_close) {
//        ofstream ctable(out, ios_base::app );
//        ctable << "</table>\n";
//        ctable.clear();
//    }
//
//
//}
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//void printParamList() {
//    cout << "Test image erosion parallel performance with openMP.";
//    cout << "\n";
//    cout << "Usage:   command <INPUT_IMAGE> [options] \n\n";
//    cout << "NB: options between two rows are mutually exclusive (use max one of them) \n\n";
//    cout << "         LONG          |    SHORT     -    DESCRIPTION                                      DEFAULT   \n";
//    cout << " ---------------------------------------------------------------------------------------------------  \n";
//    cout << " iter         <NUM>    |  i  <NUM>    -   Specify the number NUM of test to do              {100}     \n";
//    cout << " ---------------------------------------------------------------------------------------------------  \n";
//    cout << " no-preliminar         |  np          -   Disable the preliminar triple test run            {enabled} \n";
//    cout << " ---------------------------------------------------------------------------------------------------  \n";
//    cout << " fixed-thread  <N>     |  ft <NUM>    -   Specify the number of thread to use               {disabled}\n";
//    cout << " div-thread    <N>     |  dt <NUM>    -   Specify the number of threads as imgRows/NUM      {8}       \n";
//    cout << "                       |                  Specify ft or dt multiple time to run multiple tests         \n";
//    cout << " ---------------------------------------------------------------------------------------------------  \n";
//    cout << " se-wh      <NUM>      |  swh <W> <H> -   Set the width and height of Structuring Element   {7}       \n";
//    cout << " se-dim     <NUM>      |  sd  <NUM>   -   Set same width and height for Structuring Element {7}       \n";
//    cout << "                       |                  Specify se-dim  or se-wh multiple time to run multiple tests\n";
//    cout << " ---------------------------------------------------------------------------------------------------  \n";
//    cout << " table-out    <PATH>   |  to <PATH>   -   Create new html-table, write results and close    {disabled}\n";
//    cout << " table-new    <PATH>   |  tn <PATH>   -   Create new html-table, write results, leave open  {disabled}\n";
//    cout << " table-append <PATH>   |  ta <PATH>   -   Append new results to an opened table             {disabled}\n";
//    cout << " table-close  <PATH>   |  tc <PATH>   -   Append new results to an opened table and close   {disabled}\n";
//    cout << " ---------------------------------------------------------------------------------------------------  \n";
//    cout << " terminal              |  t           -   Show output in terminal                           {disabled}\n";
//    cout << " verbose               |  v           -   Show verbose output in terminal                   {disabled}\n";
//    cout << " ---------------------------------------------------------------------------------------------------  \n";
//    cout << " show-img              |  s           -   Show output image of the first erosion            {disabled}\n";
//    cout << " ---------------------------------------------------------------------------------------------------  \n";
//    cout << "\n\n";
//}
//
//
//
//
//bool argcheck(const char* arg, const char* long_param, const char* short_param) {
//    if(strcmp(arg, short_param ) == 0 || strcmp(arg, long_param ) == 0)
//        return true;
//    else return false;
//}
//
//void processParam(int argc, char** argv)
//{
//
//    if(argc < 2) {
//        printParamList();
//        exit(0);
//    }
//    img = argv[1];
//
//    for(int i = 2; i < argc; i++)
//    {
//        if (argcheck(argv[i], "iter", "i"))  niter = atoi(argv[++i]);
//
//        else if (argcheck(argv[i], "no-preliminar", "np"))   preliminar_triple_run = false;
//
//
//        else if (argcheck(argv[i], "fixed-thread", "ft"))   fixedThreadList.push_back(atoi(argv[++i]));
//        else if (argcheck(argv[i], "div-thread", "dt"))     divThreadList.push_back(atoi(argv[++i]));
//
//
//        else if (argcheck(argv[i], "se-dim", "sd")) {
//            uint dim = atoi(argv[++i]);
//            tuple<uint, uint> dims(dim, dim);
//            seDimsList.push_back( dims );
//        }
//        else if (argcheck(argv[i], "se-wh", "swh")) {
//            uint w = atoi(argv[++i]);
//            uint h = atoi(argv[++i]);
//            tuple<uint, uint> dims(w, h);
//            seDimsList.push_back( dims );
//        }
//
//
//
//        else if (argcheck(argv[i], "table-out", "to")) {
//            table_create = table_append= table_close = true;
//            out = argv[++i];
//        }
//        else if (argcheck(argv[i], "table-new", "tn")) {
//            table_create = true;
//            out = argv[++i];
//        }
//        else if(argcheck(argv[i], "table-append", "ta")) {
//            table_append = true;
//            out = argv[++i];
//        }
//        else if(argcheck(argv[i], "table-close", "tc") ) {
//            table_append = table_close = true;
//            out = argv[++i];
//        }
//
//
//        else if(argcheck(argv[i], "terminal", "t") ) terminal = true;
//        else if(argcheck(argv[i], "verbose",  "v") ) terminal = verbose = true;
//        else if(argcheck(argv[i], "show-img", "s") ) show_img = true;
//
//
//
//
//        else if(argcheck(argv[i], "help", "h")
//                || argcheck(argv[i], "-help", "-h")
//                || argcheck(argv[i], "--help", "--h")
//                || argcheck(argv[i], "-?", "?"))
//        {
//            printParamList();
//            exit(0);
//        }
//
//        else {
//            cout << "INVALID PARAMETER '" << argv[i] << "' AT POSITION: " << i-1 << std::endl << std::endl;
//            printParamList();
//            exit(1);
//        }
//
//    }
//}
