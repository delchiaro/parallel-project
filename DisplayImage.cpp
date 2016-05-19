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


    Image image(argv[1]);

//
//    Image immersion = image.makeImmersion(10,13,20,30, 0);
//    immersion.imshow("ImmersionTest");
//


    int rows = 6;
    int cols = 6;
    imProc::Matrix<bool> SE(cols, rows, true);

    imProc::Point<int> center( (int)ceil(rows/2), (int)ceil(cols/2));


    TimeProfiler t;

    Image Eroded(image);
    t.start();
    Eroded.erosion(SE, center );
    t.stop();
    cout << "Erosion: " << t << endl;

    Image Eroded1(image);
    t.start();
    Eroded.erosion(SE, center );
    t.stop();
    cout << "Erosion: " << t << endl;

    Image Eroded2(image);
    t.start();
    Eroded.erosion(SE, center );
    t.stop();
    cout << "Erosion: " << t << endl;





    //cout << endl << endl << "DONE!" << endl << "Press a key to continue. . .";

    return 0;
}


