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
#include <stdio.h>
#include <stdlib.h>

using namespace std;

namespace convolutionBench
{


class BenchManager {

public:
    BenchManager( int argc, char** argv );

    template <class T> int start(T& bench)
    {

        std::string timeUnit;
        if( secondsMult ==  1)
            timeUnit = "S";
        else if( secondsMult == 1000)
            timeUnit = "mS";
        else if( secondsMult == 1000000)
            timeUnit = "uS";
        else if( secondsMult == 1000000000)
            timeUnit = "nS";
        else timeUnit += "S*" + std::to_string(secondsMult);

        if(html_table_create)
        {
                table_html.open(out_html, ios_base::trunc );
                table_html << "<table border=\"1px solid black\" style=\"border: 1px solid black; border-collapse: collapse\">\n\n";

                table_html << "\t<tr>\n";
                table_html << "\t\t<th>SE w</th>\n";
                table_html << "\t\t<th>SE h</th>\n";
                table_html << "\t\t<th>Threads</th>\n";
//                if(preliminar_run > 0 && preliminar_out)
//                {
//                    table_html << "\t\t<th>Run 1</th>\n";
//                    table_html << "\t\t<th>Run 2</th>\n";
//                    table_html << "\t\t<th>Run 3</th>\n";
//                }

            table_html << "\t\t<th>Loops</th>\n"; // new

            table_html << "\t\t<th>Benchs</th>\n"; // new
            table_html << "\t\t<th>Avarage [" << timeUnit << "]</th>\n"; // the old one
            table_html << "\t\t<th>Variance</th>\n";// new
            table_html << "\t</tr>\n\n";
        }
        if(csv_table_create)
        {
            table_csv.open(out_csv, ios_base::trunc );
            table_csv << "\"SE w\", ";
            table_csv << "\"SE h\", ";
            table_csv << "\"Threads\", ";
//            if(preliminar_run > 0 && preliminar_out)
//            {
//                table_csv << "\"Run 1\", ";
//                table_csv << "\"Run 2\", ";
//                table_csv << "\"Run 3\", ";
//            }
            table_csv << "\"Loops\", ";// new
            table_csv << "\"Benchs\", ";// new
            table_csv << "\"Avarage [" << timeUnit << "]\", "; // the old one
            table_csv << "\"Variance\", ";// new
        }



        if( seDimsList.empty() )
            seDimsList.push_back( tuple<uint, uint>(DEFAULT_SE_DIM, DEFAULT_SE_DIM)  );

        if( divThreadList.empty() && fixedThreadList.empty() )
            divThreadList.push_back( DEFAULT_DIV_THREAD );



        if(preliminar_global > 0)
        {
            bench.init(imgPath, fixedThreadList.front(), get<0>(seDimsList.front()), get<1>(seDimsList.front()), true);
            bench.start(preliminar_global);
        }
        int nSE = seDimsList.size();
        for (int sed = 0; sed < seDimsList.size(); sed++)
        {
            tuple<uint, uint> dims = seDimsList.front();
            seDimsList.pop_front();
            uint se_w = get<0>(dims);
            uint se_h = get<1>(dims);
            ofstream table;

            std::list<uint> fixedThreadList_sed = fixedThreadList;
            // for each sed (se-dimension) we have to iterate all the fixedTHreadList
            // so we make a copy and we pop the elements from the copy.
            while(fixedThreadList_sed.empty() == false)
            {
                uint fixedThread = fixedThreadList_sed.front();
                fixedThreadList_sed.pop_front();

                if(html_table_append)
                {
                    if(table_html.is_open() == false)
                        table_html.open(out_html, ios_base::app);
                    table_html << "\t<tr>\n";
                    //table_html << "\t\t<td colspan=\"" << divThreadList.size() + fixedThreadList.size() << "\">" << se_w << "</td>\n";
                    //table_html << "\t\t<td colspan=\"" << divThreadList.size() + fixedThreadList.size()  << "\">" << se_w << "</td>\n";
                    table_html << "\t\t<td>" << se_w << "</td>\n";
                    table_html << "\t\t<td>" << se_w << "</td>\n";
                    table_html << "\t\t<td>" << fixedThread << "</td>\n";
                }
                if(csv_table_append)
                {
                    if(table_csv.is_open() == false)
                        table_csv.open(out_csv, ios_base::app);
                    table_csv << "\n";
                    table_csv << se_w << ", ";
                    table_csv << se_w << ", ";
                    table_csv << fixedThread << ", ";
                }

                runBench<T>(bench, fixedThread, 0, get<0>(dims), get<1>(dims));


                if(html_table_append) table << "\t</tr>\n";
            }


        }

        if(html_table_close)
            table_html << "</table>\n";

        if(table_html.is_open())
            table_html.close();

        if(table_csv.is_open())
            table_csv.close();

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


    // TODO: should be IConvBench<T> convolutionBench, not T convolutionBench.. but errors occur at compile time..
    template <class T> void runBench(T& bench, const uint& threads, const uint& div_thread, const uint& se_width, const uint& se_height)
    {

        if(verbose) {
            cout << "Version: Object Oriented " << endl;
            cout << "OpenMP version: " << _OPENMP << endl;
        }


        int rows = bench.getImgHeight();
        bool useThreadsAsDivisor = false;
//        if(div_thread > 0)
//            bench.init(imgPath, div_thread, se_width, se_height, true);
//        else
        bench.init(imgPath, threads, se_width, se_height, false);



        TimeProfiler t;
        cout << "N Threads = " << bench.getThreads() << "\n";

        double T1 = 0, T2 = 0, T3 = 0;
        for( int i=0 ; i < preliminar_run ; i++)
        {
            t = bench.start();
            if(verbose) cout << "Preliminar Bench: " << t << endl;
            else if(terminal) cout << t << endl;
            T1 = t.getDSeconds();
        }


        // incremental avarage computing:
        // http://math.stackexchange.com/questions/106700/incremental-averageing
        // avg[n] = avg[n-1] + ( (value[n]-avg[n-1]) / n )
        double avg = 0;
        double ms[benchNumber];
        for(int i = 0; i < benchNumber ; i++)
        {
            if (benchLoops > 1)
                t = bench.start(benchLoops);
            else t = bench.start();
            ms[i] = t.getDSeconds()*secondsMult;
            avg = avg + (ms[i] - avg) / (i+1);
        }

        // https://en.wikipedia.org/wiki/Algorithms_for_calculating_variance#Two-pass_algorithm
        // variance =  SUM i=1->n { (value[i] - avg )^2 } / (n-1)
        double variance = 0;
        if(benchNumber > 2)
        {
            for (int i = 0; i < benchNumber; i++)
                variance += (ms[i] - avg) * (ms[i] - avg);
            variance /= benchNumber-1;
        }
        if(verbose) cout << "Bench x" << benchLoops << ": " << t << endl << endl;
        else if(terminal) cout << t << endl << endl;




        if(html_table_append)
        {
            //ofstream table(out, ios_base::app );
//            if( preliminar_run > 0 && preliminar_out)
//            {
//                table_html << "\t\t<td>" << T1 << "</td>\n";
//                table_html << "\t\t<td>" << T2 << "</td>\n";
//                table_html << "\t\t<td>" << T3 << "</td>\n";
//            }

            table_html << "\t\t<td>" << benchLoops << "</td>\n";
            table_html << "\t\t<td>" << benchNumber << "</td>\n";
            table_html << "\t\t<td>" << avg << "</td>\n";
            table_html << "\t\t<td>" << variance << "</td>\n";

            // table_html.close();
        }
        if(csv_table_append)
        {
//            if( preliminar_run > 0 && preliminar_out)
//            {
//                table_csv << T1 << ", ";
//                table_csv << T2 << ", ";
//                table_csv << T3 << ", ";
//            }
            table_csv << benchLoops <<", ";
            table_csv << benchNumber <<", ";
            table_csv << avg <<", ";
            table_csv << variance <<", ";
        }



    }


private:

    std::ofstream table_html;
    std::ofstream table_csv;

    uint DEFAULT_BENCH_LOOPS = 1;
    uint DEFAULT_BENCH_NUMBER = 100;

    uint DEFAULT_SE_DIM = 7;
    uint DEFAULT_DIV_THREAD = 8;

    std::string out_html;
    std::string out_csv;

    bool csv_table_create = false;
    bool csv_table_append = false;

    bool html_table_create = false;
    bool html_table_append = false;
    bool html_table_close  = false;



    bool terminal = false;
    bool verbose = false;
    bool showImg = false;

    int  preliminar_global = 50;
    int  preliminar_run = 5;
    bool preliminar_out = false;

    uint benchLoops = DEFAULT_BENCH_LOOPS;
    uint benchNumber = DEFAULT_BENCH_NUMBER;

    std::string imgPath;

    long secondsMult = 1;

    std::list<uint> divThreadList;
    std::list<uint> fixedThreadList;
    std::list< std::tuple<uint, uint> > seDimsList;


    void printParamList();
    void processParam(int argc, char** argv);
    bool argcheck(const char* arg, const char* long_param, const char* short_param);

};


}

#endif //STRUCTURED_ACOMMANDLINEMAIN_H
