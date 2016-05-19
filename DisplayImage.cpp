#include <iostream>
#include <stdio.h>
#include <opencv2/opencv.hpp>

#include "TimeProfiler.h"

#include "imProc/Point.h"
#include "imProc/Matrix.h"
#include "imProc/Image.h"



using namespace std;

typedef imProc::Image<uchar> Image;


int main(int argc, char** argv ) {
    if ( argc != 2 )
    {
        printf("usage: DisplayImage.out <Image_Path>\n");
        return -1;
    }


    cout << "OpenMP version: " << _OPENMP << endl;

    Image image(argv[1]);
    //image.imshow("Original Image");

//
//    Image immersion = image.makeImmersion(10,13,20,30, 0);
//    immersion.imshow("ImmersionTest");
//


    int rows = 6;
    int cols = 6;
    imProc::Matrix<bool> SE(cols, rows, true);

    imProc::Point<int> center( (int)ceil(rows/2), (int)ceil(cols/2));


    TimeProfiler t;



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

    Image eroded(image);
    t.start();
    eroded.erosion(SE, center);
    t.stop();
    cout << "Erosion: " << t << endl;


    Image eroded1(image);
    t.start();
    eroded1.erosion(SE, center);
    t.stop();
    cout << "Erosion: " << t << endl;


    Image eroded2(image);
    t.start();
    eroded2.erosion(SE, center);
    t.stop();
    cout << "Erosion: " << t << endl;


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


    //cout << endl << endl << "DONE!" << endl << "Press a key to continue. . .";
   // cv::waitKey(0);

    return 0;
}


