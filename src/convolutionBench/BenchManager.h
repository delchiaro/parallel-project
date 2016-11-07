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
    std::string timeUnit;

public:
    BenchManager( int argc, char** argv );

    template <class T> int start(T& bench)
    {
        // bench.init(imgPath, fixedThreadList.front(), get<0>(seDimsList.front()), get<1>(seDimsList.front()), true);

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
            if(truncateFile)
                table_html.open(out_html, ios_base::trunc | ios_base::out );
            else
                table_html.open(out_html, ios_base::app   | ios_base::out );

            table_html << "\n";
            table_html << "<h4>" << bench.getBenchName() << "</h4>\n";
            table_html << "<table border=\"1px solid black\" style=\"border: 1px solid black; border-collapse: collapse\">\n\n";

            table_html << "\t<tr>\n";
            table_html << "\t\t<th>SE w</th>\n";
            table_html << "\t\t<th>SE h</th>\n";
            table_html << "\t\t<th>Threads</th>\n";

            table_html << "\t\t<th>Loops</th>\n";

            table_html << "\t\t<th>Benchs</th>\n";
            table_html << "\t\t<th>Avarage [" << timeUnit << "]</th>\n";
            if(useVariance)
                table_html << "\t\t<th>Variance</th>\n";
            else table_html << "\t\t<th>StdDev</th>\n";
            if(printAllBenchs)
            {
                table_html << "\t\t<th></th>\n";
                for(int i=0 ; i<benchNumber ; i++)
                    table_html << "\t\t<th>#" << i+1 << "</th>\n";

            }
            table_html << "\t</tr>\n\n";
        }
        if(csv_table_create)
        {
            if(truncateFile)
                 table_csv.open(out_csv, ios_base::trunc | ios_base::out );
            else table_csv.open(out_csv, ios_base::app   | ios_base::out );

            //table_csv << "\n\n";
            table_csv << "\"" << bench.getBenchName() << "\", ";
            table_csv << "\"SE w\", ";
            table_csv << "\"SE h\", ";
            table_csv << "\"Threads\", ";
            table_csv << "\"Loops\", ";
            table_csv << "\"Benchs\", ";
            table_csv << "\"Avarage [" << timeUnit << "]\", ";
            if(useVariance)
                table_csv << "\"Variance\", ";
            else table_csv << "\"StdDev\", ";

            if(printAllBenchs)
            {
                table_csv << ", ";
                for(int i=0 ; i<benchNumber ; i++)
                    table_csv << "#" << i+1 << ", ";
            }
        }



        if( seDimsList.empty() )
            seDimsList.push_back( tuple<uint, uint>(DEFAULT_SE_DIM, DEFAULT_SE_DIM)  );

        if( fixedThreadList.empty() )
            fixedThreadList.push_back( DEFAULT_DIV_THREAD );



        if(preliminar_global > 0)
        {
            bench.init(imgPath, fixedThreadList.front(), get<0>(seDimsList.front()), get<1>(seDimsList.front()), true);
            bench.start(preliminar_global);
        }

        while(seDimsList.empty() == false)
        {

            tuple<uint, uint> dims = seDimsList.front();
            seDimsList.pop_front();
            uint se_w = get<0>(dims);
            uint se_h = get<1>(dims);

            ofstream table;

            std::list<uint> currentFixedThreadList = fixedThreadList;
            // for each sed (se-dimension) we have to iterate all the fixedTHreadList
            // so we make a copy and we pop the elements from the copy.

            while(currentFixedThreadList.empty() == false)
            {
                uint fixedThread = currentFixedThreadList.front();
                currentFixedThreadList.pop_front();

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
                    table_html.close();
                }
                if(csv_table_append)
                {
                    if(table_csv.is_open() == false)
                        table_csv.open(out_csv, ios_base::app);
                    table_csv << "\n";
                    table_csv <<", "; // first row is the name of the bench!
                    table_csv << se_w << ", ";
                    table_csv << se_w << ", ";
                    table_csv << fixedThread << ", ";
                    table_csv.close();
                }

                runBench<T>(bench, fixedThread, 0, get<0>(dims), get<1>(dims));


                if(html_table_append) {
                    if(table_html.is_open() == false)
                        table_html.open(out_html, ios_base::app);
                    table_html << "\t</tr>\n";
                    table_html.close();
                }
            }

        }

        if(html_table_close) {
            if(table_html.is_open() == false)
                table_html.open(out_html, ios_base::app);
            table_html << "</table>\n<br>\n\n";
            table_html.close();
        }

        if(csv_table_append)
        {
            if(table_csv.is_open() == false)
                table_csv.open(out_csv, ios_base::app);
            table_csv << "\n\n";
            table_csv.close();
        }


        // Useless check ??
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

        int rows = bench.getImgHeight();
        bool useThreadsAsDivisor = false;
//        if(div_thread > 0)
//            bench.init(imgPath, div_thread, se_width, se_height, true);
//        else
        bench.init(imgPath, threads, se_width, se_height, false);

        if(terminal) {
            cout << "Bench Name: " << bench.getBenchName() << endl;
            cout << "SE W: " << bench.getSeHeight() << " \t" << "SE H:" << bench.getSeWidth() << "\n";
            cout << "N Threads = " << bench.getThreads() << "\n";
        }

        TimeProfiler t;

        double T1 = 0, T2 = 0, T3 = 0;
        for( int i=0 ; i < preliminar_run ; i++)
        {
            t = bench.start();
            if(verbose) cout << "Preliminar Bench: " << t << endl;
        }


        // incremental avarage computing:
        // http://math.stackexchange.com/questions/106700/incremental-averageing
        // avg[n] = avg[n-1] + ( (value[n]-avg[n-1]) / n )
        double avg = 0;
        double times[benchNumber];
        for(int i = 0; i < benchNumber ; i++)
        {
            if (benchLoops > 1)
                t = bench.start(benchLoops);
            else t = bench.start();
            times[i] = t.getDSeconds()*secondsMult;
            if(timePerLoop)
                times[i] /= benchLoops;
            avg = avg + (times[i] - avg) / (i+1);
        }

        // https://en.wikipedia.org/wiki/Algorithms_for_calculating_variance#Two-pass_algorithm
        // variance =  SUM i=1->n { (value[i] - avg )^2 } / (n-1)
        double stdDev = 0;
        if(benchNumber > 2)
        {
            for (int i = 0; i < benchNumber; i++)
                stdDev += (times[i] - avg) * (times[i] - avg);
            stdDev /= benchNumber-1;
            if(useVariance == false)
                stdDev = sqrt(stdDev);

        }
        if(verbose) {
            cout << "---------------------------------------" << endl;
            cout << "Bench x" << benchNumber << " (" << benchLoops << " loop per bench)" << endl;
            cout << "---------------------------------------" << endl;
            cout << "    AVG = " << avg << " [" << timeUnit << "] ";
            if(timePerLoop) cout << "(time per loop)";
            else cout << "(time per bench = " << benchLoops << " loops)";
            cout << endl;
            if(useVariance)
                 cout << "    VAR = " << stdDev << endl;
            else cout << " StdDEV = " << stdDev << endl;
            cout << "---------------------------------------" << endl << endl << endl ;
        }



        if(html_table_append)
        {
            if(table_html.is_open() == false)
                table_html.open(out_html, ios_base::app);
            table_html << "\t\t<td>" << benchLoops << "</td>\n";
            table_html << "\t\t<td>" << benchNumber << "</td>\n";
            table_html << "\t\t<td>" << avg << "</td>\n";
            table_html << "\t\t<td>" << stdDev << "</td>\n";
            if(printAllBenchs) {
                table_html << "\t\t<td></td>\n";
                for(int i=0 ; i<benchNumber ; i++)
                    table_html << "\t\t<td>"  << times[i] << "</td>\n";
            }
            table_html.close();
        }
        if(csv_table_append)
        {
            if(table_csv.is_open() == false)
                table_csv.open(out_csv, ios_base::app);
            table_csv << benchLoops <<", ";
            table_csv << benchNumber <<", ";
            table_csv << avg <<", ";
            table_csv << stdDev <<", ";
            if(printAllBenchs) {
                table_csv << ", ";
                for(int i=0 ; i<benchNumber ; i++)
                    table_csv << times[i] << ", ";
            }
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


    bool timePerLoop = true;
    bool truncateFile = false;
    bool printAllBenchs = false;
    bool useVariance = false;
    bool terminal = true;
    bool verbose = false;
    bool showImg = false;

    int  preliminar_global = 0;
    int  preliminar_run = 0;
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
    bool argcheck(const char* arg, const char* long_param, const char* short_param, const char* short_param2);

    inline bool file_exists_test (const std::string& name) {
        ifstream f(name.c_str());
        return f.good();
    }
};


}

#endif //STRUCTURED_ACOMMANDLINEMAIN_H
