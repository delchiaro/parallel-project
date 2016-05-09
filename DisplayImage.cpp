#include <iostream>
#include <stdio.h>
#include <opencv2/opencv.hpp>


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

    Image image = *Image::newImageCV(argv[1]);
    image.imshow("Original Image");

    Image copy = *image.clone();
    copy.imshow("clone image");

    int rows = 6;
    int cols = 6;
    imProc::Matrix<bool> SE(cols, rows, true);

    imProc::Point<int> center( (int)ceil(cols/2), (int)ceil(rows/2));

    Image open = * image.opening(SE, center );
    //namedWindow("Opened Image", WINDOW_AUTOSIZE );
    open.imshow("Opened Image");


    Image closed = *image.closure( SE, center );
    //namedWindow("Closed Image", WINDOW_AUTOSIZE );
    closed.imshow("Closed Image");

    Image closedOpened = *open.closure(SE, center);
    closedOpened.imshow("Closed Opened Image");


    cv::waitKey(0);

    return 0;
}


