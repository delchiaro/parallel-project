#include <iostream>
#include <opencv2/opencv.hpp>
#include <fstream>
#include <list>
#include <tuple>


#include "__deprecated/commonCommandLine.cpp"
#include "utils/TimeProfiler.h"

#include "imProc/Point.h"
#include "imProc/Matrix.h"
#include "imProc/Image.h"
#include "convolutionBench/IConvBench.h"
#include "benchOOP/BenchOOP.h"
#include "convolutionBench/BenchManager.h"
#include "benchOOP/ClosureBenchOOP.h"
#include "benchOOP/ErosionBenchOOP.h"
#include "benchOOP/OpeningBenchOOP.h"


using namespace std;



using namespace convolutionBench;

int main(int argc, char** argv ) {
    BenchManager benchManager(argc, argv);

//    ClosureBenchOOP bench;
//    benchManager.start<ClosureBenchOOP>(bench);

    ErosionBenchOOP bench;
    benchManager.start<ErosionBenchOOP>(bench);

}

//
//void printParamList();
//void processParam(int argc, char** argv);
//bool argcheck(const char* arg, const char* long_param, const char* short_param);
//void run(int fixed_thread, int div_thread, uint se_width, uint se_height);
//


//
//void run(int fixed_thread, int div_thread, uint se_width, uint se_height)
//{
//
//    if(verbose) {
//        cout << "Version: Object Oriented " << endl;
//        cout << "OpenMP version: " << _OPENMP << endl;
//    }
//    Image image(img);
//    image.setRowsThreadDivisor(div_thread);
//    image.setThreads(fixed_thread);
//
//    imProc::Matrix<bool> SE(se_height, se_width, true);
//    imProc::Point<int> center( (int)ceil(se_height/2), (int)ceil(se_width/2) );
//
//
//    TimeProfiler t;
//
//    Image eroded1(image);
//    cout << "N Threads = " << eroded1.getNumberOfThreads() << "\n";
//
//         double T1 = 0, T2 = 0, T3 = 0;
//    if( preliminar_triple_run )
//    {
//        //Image eroded1(image);
//        t.start();
//        eroded1.erosion(SE, center);
//        t.stop();
//        if(verbose) cout << "Erosion: " << t << endl;
//        else if(terminal) cout << t << endl;
//        T1 = t.getDSeconds();
//
//        Image eroded2(image);
//        t.start();
//        eroded2.erosion(SE, center );
//        t.stop();
//        if(verbose) cout << "Erosion: " << t << endl;
//        else if(terminal) cout << t << endl;
//        T2 = t.getDSeconds();
//
//        Image eroded3(image);
//        t.start();
//        eroded3.erosion(SE, center );
//        t.stop();
//        if(verbose) cout << "Erosion: " << t << endl;
//        else if(terminal) cout << t << endl;
//        T3 = t.getDSeconds();
//    }
//
//
//
//
//    Image convolutionBench(image);
//
//    t.start();
//    for( int i = 0; i < niter; i++ )
//    {
//        convolutionBench.erosion(SE, center);
//    }
//    t.stop();
//    if(verbose) cout << "Erosion IConvBench (erosion x " << niter << "): " << t << endl;
//    else if(terminal) cout << t << endl;
//    double TLOOP = t.getDSeconds();
//
//
//
//
//
//
//    if(table_append)
//    {
//        ofstream table(out, ios_base::app );
//        table << "\t\t<td>" << T1 << "</td>\n";
//        table << "\t\t<td>" << T2 << "</td>\n";
//        table << "\t\t<td>" << T3 << "</td>\n";
//        table << "\t\t<td>" << TLOOP << "</td>\n";
//        table.close();
//    }
//
//
//
//
//
//    if(show_img) {
//        if(preliminar_triple_run == false)
//            eroded1.erosion(SE, center);
//        cout << endl << endl;
//        image.imshow("Original");
//        eroded1.imshow("First Erosion");
//        cout << endl << endl << "DONE!" << endl << "Press a key to continue. . .";
//        cv::waitKey(0);
//    }
//
//
//    return;
//}
//
//
//
//















//    Image immersion = image.makeImmersion(10,13,20,30, 0);
//    immersion.imshow("ImmersionTest");



//    Image dilatedd(image);
//    t.start();
//    dilatedd.dilation(SE, center);
//    t.stop();
//    cout << "Dilation: " << t << endl;
//
//
//    Image dilated(image);
//    t.start();
//    dilated.dilation(SE, center);
//    t.stop();
//    cout << "Dilation: " << t << endl;


//    cv::Mat Display = Eroded.toMatCV();
//
//    cv::imshow("Closed Opened Image", Display);
//
//
//    cv::waitKey(0);

//
//    t.start();
//    open.opening(SE, center );
//    t.stop();
//    cout << "Opening: " << t << endl;
//    open.imshow("Opened Image");
//
//
//    Image close(image);
//    t.start();
//    close.closure( SE, center );
//    t.stop();
//    cout << "Closing: " << t << endl;
//    close.imshow("Closed Image");
//
//    open.closure(SE, center);
//    open.imshow("Closure on Opened Image");
//
//    close.opening(SE, center);
//    close.imshow("Opening on Closed Image");
//



