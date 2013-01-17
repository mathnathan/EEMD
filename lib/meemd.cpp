#include "meemd.h"
#include <opencv2/opencv.hpp>
#include <string>
#include <stdio.h>

using namespace cv;


//----------------------------------------------------------------------
bool MEEMD::load( const string& fileName ) 
{
    signal = imread( fileName );
    if( !signal.data ) // Check for invalid input
    {
        printf("Could not open or find the image\n");
        return false;
    } 

    return true;
}
//----------------------------------------------------------------------
void MEEMD::show()
{
    const string windowName = "Signal";
    namedWindow( windowName, CV_WINDOW_AUTOSIZE );
    //imshow( windowName, signal );
    //waitKey(0);
}
//----------------------------------------------------------------------
VEC MEEMD::decompose( MAT& signal, double level, int dimension )
{
}
//----------------------------------------------------------------------
MAT MEEMD::combine()
{
}
