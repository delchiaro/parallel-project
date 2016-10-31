//
// Created by nagash on 21/10/16.
//

#ifndef STRUCTURED_ACOMMANDLINEMAIN_H
#define STRUCTURED_ACOMMANDLINEMAIN_H

#include "../utils/utils.h"
#include "IConvBench.h"
#include <opencv2/opencv.hpp>
#include <list>
#include <tuple>
#include <fstream>
using namespace std;

namespace convolutionBench
{


class BenchManager {

public:
    BenchManager( int argc, char** argv );

    template <class T> int start(T& bench)
    {

        if(table_create)
        {
            table.open(out, ios_base::trunc );
            table << "<table border=\"1px solid black\" style=\"border: 1px solid black; border-collapse: collapse\">\n\n";

            table << "\t<tr>\n";
            table << "\t\t<th>SE w</th>\n";
            table << "\t\t<th>SE h</th>\n";
            table << "\t\t<th>DivThr</th>\n";
            table << "\t\t<th>FixThr</th>\n";
            if(preliminar_triple_run)
            {
                table << "\t\t<th>Run 1</th>\n";
                table << "\t\t<th>Run 2</th>\n";
                table << "\t\t<th>Run 3</th>\n";
            }
            table << "\t\t<th>Loop x" << DEFAULT_BENCH_LOOPS << "</th>\n";
            table << "\t</tr>\n\n";
        }


        if( seDimsList.empty() )
            seDimsList.push_back( tuple<uint, uint>(DEFAULT_SE_DIM, DEFAULT_SE_DIM)  );

        if( divThreadList.empty() && fixedThreadList.empty() )
            divThreadList.push_back( DEFAULT_DIV_THREAD );



        for (int sed = 0; sed < seDimsList.size(); sed++)
        {
            tuple<uint, uint> dims = seDimsList.front();
            uint se_w = get<0>(dims);
            uint se_h = get<0>(dims);
            ofstream table;

            while(divThreadList.empty() == false)
            {
                uint divThread = divThreadList.front();
                divThreadList.pop_front();

                if(table_append) {
                    if(table.is_open() == false)
                        table.open(out, ios_base::app);
                    table << "\t<tr>\n";
                    //table << "\t\t<td colspan=\"" << divThreadList.size() + fixedThreadList.size() << "\">" << se_w << "</td>\n";
                    //table << "\t\t<td colspan=\"" << divThreadList.size() + fixedThreadList.size()  << "\">" << se_w << "</td>\n";
                    table << "\t\t<td>" << se_w << "</td>\n";
                    table << "\t\t<td>" << se_w << "</td>\n";
                    table << "\t\t<td>" << divThread << "</td>\n";
                    table << "\t\t<td></td>\n";
                }

                runBench<T>(bench, 0, divThread, get<0>(dims), get<1>(dims));

                if(table_append)
                    table << "\t</tr>\n";
            }

            while(fixedThreadList.empty() == false)
            {
                uint fixedThread = fixedThreadList.front();
                fixedThreadList.pop_front();

                if(table_append) {
                    if(table.is_open() == false)
                        table.open(out, ios_base::app);
                    table << "\t<tr>\n";
                    //table << "\t\t<td colspan=\"" << divThreadList.size() + fixedThreadList.size() << "\">" << se_w << "</td>\n";
                    //table << "\t\t<td colspan=\"" << divThreadList.size() + fixedThreadList.size()  << "\">" << se_w << "</td>\n";
                    table << "\t\t<td>" << se_w << "</td>\n";
                    table << "\t\t<td>" << se_w << "</td>\n";
                    table << "\t\t<td></td>\n";
                    table << "\t\t<td>" << fixedThread << "</td>\n";
                }

                runBench<T>(bench, fixedThread, 0, get<0>(dims), get<1>(dims));


                if(table_append) table << "\t</tr>\n";
            }


        }

        if(table_close)
            table << "</table>\n";
        if(table.is_open())
            table.close();

        if(showImg)
        {
            //if(preliminar_triple_run == false)
            //    singleBench();
            cout << endl << endl;
            bench.showOriginalImg();
            bench.showProcessedImg();
            //image.imshow("Original");
            //eroded1.imshow("First Erosion");
            cout << endl << endl << "DONE!" << endl << "Press a key to continue. . .";
            cv::waitKey(0);
        }

    }


protected:

    bool isOutputTerminalOn() { return terminal; }
    bool isOutputVerboseOn() { return verbose; }
    bool isShowImgOn() { return showImg; }

    bool isPreliminarTripleRunOn() { return preliminar_triple_run; }


    // TODO: should be IConvBench<T> convolutionBench, not T convolutionBench.. but errors occur at compile time..
    template <class T> void runBench(T& bench, const uint& threads, const uint& div_thread, const uint& se_width, const uint& se_height)
    {

        if(verbose) {
            cout << "Version: Object Oriented " << endl;
            cout << "OpenMP version: " << _OPENMP << endl;
        }


        int rows = bench.getImgHeight();
        bool useThreadsAsDivisor = false;
        if(div_thread > 0)
            bench.init(imgPath, div_thread, se_width, se_height, true);
        else bench.init(imgPath, threads, se_width, se_height, false);



        TimeProfiler t;
        cout << "N Threads = " << bench.getThreads() << "\n";

        double T1 = 0, T2 = 0, T3 = 0;
        if( preliminar_triple_run )
        {
            t = bench.start();
            if(verbose) cout << "Bench: " << t << endl;
            else if(terminal) cout << t << endl;
            T1 = t.getDSeconds();

            t = bench.start();
            if(verbose) cout << "Bench: " << t << endl;
            else if(terminal) cout << t << endl;
            T2 = t.getDSeconds();

            t = bench.start();
            if(verbose) cout << "Bench: " << t << endl;
            else if(terminal) cout << t << endl;
            T3 = t.getDSeconds();
        }


        t = bench.start(benchLoops);
        if(verbose) cout << "Bench x" << benchLoops << ": " << t << endl << endl;
        else if(terminal) cout << t << endl << endl;
        double TLOOP = t.getDSeconds();




        if(table_append)
        {
            ofstream table(out, ios_base::app );
            table << "\t\t<td>" << T1 << "</td>\n";
            table << "\t\t<td>" << T2 << "</td>\n";
            table << "\t\t<td>" << T3 << "</td>\n";
            table << "\t\t<td>" << TLOOP << "</td>\n";
            table.close();
        }




    }


private:

    std::ofstream table;

    uint DEFAULT_BENCH_LOOPS = 100;
    uint DEFAULT_SE_DIM = 7;
    uint DEFAULT_DIV_THREAD = 8;



    std::string out;
    bool table_create = false;
    bool table_append = false;
    bool table_close  = false;



    bool terminal = false;
    bool verbose = false;
    bool showImg = false;


    bool preliminar_triple_run = true;
    uint benchLoops = DEFAULT_BENCH_LOOPS;

    std::string imgPath;


    std::list<uint> divThreadList;
    std::list<uint> fixedThreadList;
    std::list< std::tuple<uint, uint> > seDimsList;


    void printParamList();
    void processParam(int argc, char** argv);
    bool argcheck(const char* arg, const char* long_param, const char* short_param);

};


}

#endif //STRUCTURED_ACOMMANDLINEMAIN_H
