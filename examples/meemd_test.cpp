#include <opencv2/opencv.hpp>
#include "meemd.h"
#include "tools.h"
#include <sstream>
#include <iostream>
#include <ctime>
#include "typedefs.h"

Tools tools;

cv::Mat src, dst, detected_edges, final;

int lowThreshold;
int const max_lowThreshold = 100;
int ratio = 2;
int kernel_size = 3;
char window_name[32];

// Function: CannyThreshold
// brief Trackbar callback - Canny thresholds input with a ratio 1:3
void CannyThreshold(int min_thresh, void*) {

    lowThreshold = min_thresh;

    /// Reduce noise with a kernel 3x3
    cv::blur( src, detected_edges, cv::Size(3,3) );

    /// Canny detector
    cv::Canny( detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size );

    /// Using Canny's output as a mask, we display our result
    dst = cv::Scalar::all(0);

    src.copyTo( dst, detected_edges);

    if( !dst.data ) { printf("There is no data\n"); exit(0); }
    cv::resize( dst, final, dst.size()*3, 0, 0 );
    //cv::imshow( window_name, final );
}

int main() {

    std::ostringstream ss;

// Possible input files
    const std::string gom_crop = "../data/crop.png";
    const std::string gom_crop_sq = "../data/crop_square.png";
    const std::string gom_crop_sq1 = "../data/crop_square+1.png";
    const std::string gom = "../data/sschlor.ncom.GOMh0.04.012.nest1.2009101000_00000000.nc";
    const std::string square = "../data/simple_xy.nc";
    const std::string lena_small = "../data/lena_small.bmp";
    const std::string lena_big = "../data/lena.bmp";
// End input files

// Choose the input file here
    const std::string input = gom_crop_sq;

// TIME
    time_t t = time(0);   // get time now
    struct tm * now = localtime( & t );
    ss << now->tm_mon+1 << '-' << now->tm_mday;
    std::string date( ss.str() );
    ss.str(std::string());
// END TIME

    MEEMD m;

    if( input == gom || input == square ) {
        m.load( input, MEEMD_NETCDF );
    } 
    else {
        m.load( input, MEEMD_IMAGE );
    }

/*
// Variables for EEMD
    float noise_amplitude = 1.0;
    int nb_noise_iters = 100;

    m.decompose( noise_amplitude, nb_noise_iters );
    std::vector<MAT> results = m.combine();
    //m.show();

    std::string name = tools.split( tools.split( input, '/' )[2], '.')[0];
// Convert noise to string
    ss << noise_amplitude;
    std::string noise( ss.str() );
// End conversion
    std::string save_dir = "../results/"+date+"_"+name+"_noise="+noise+"/";
    printf("save_dir = %s\n", save_dir.c_str() );
    //m.save( save_dir );

// Find and show edges
    for( int i=0; i<results.size(); i++ ) {
        MAT tmp = results[i];

        float min = tmp.min();
        float max = tmp.max();
        printf("min = %f\n", min);
        printf("max = %f\n", max);
        float scale = 255.0/(max-min);
        MAT tmp2 = tmp-min; // Translate to 0
        tmp2 = tmp2*scale; // Scale between 0, 255


        src = cv::Mat(tmp2.n_rows,tmp2.n_cols,CV_32FC1,(void*)tmp2.memptr());
        src.convertTo(src, CV_8UC1); // Seems this is needed to show image. All white otherwise

        strcpy( window_name, "IMF" );
        cv::Mat big_src;
        cv::resize( src, big_src, src.size()*3, 0, 0 );
        cv::imshow( window_name, big_src );
        cv::waitKey(0);
        strcpy( window_name, "IMF Edges" );
        dst.create( src.size(), src.type() ); // Create a matrix of the same type and size as src (for dst)
        //cv::cvtColor( src, src_gray, CV_BGR2GRAY ); // Convert the image to grayscale
        cv::namedWindow( window_name, CV_WINDOW_AUTOSIZE ); // Create a window
        cv::createTrackbar( "Min Threshold:", window_name, &lowThreshold, max_lowThreshold, CannyThreshold ); // A Trackbar to enter threshold
        CannyThreshold(0, 0); // Show the image
        cv::waitKey(0); // Wait until user exit program by pressing a key
    }
// End Edges
*/

    cv::Mat tmp = cv::imread( gom_crop_sq );
    cv::cvtColor( tmp, src, CV_BGR2GRAY ); // Convert the image to grayscale
    //src.convertTo(src, CV_8UC1);

    strcpy( window_name, "Original Input" );
    cv::Mat big_src;
    cv::resize( src, big_src, src.size()*3, 0, 0 );
    //cv::imshow( window_name, big_src );
    cv::waitKey(0);
    dst.create( src.size(), src.type() ); // Create a matrix of the same type and size as src (for dst)
    cv::namedWindow( window_name, CV_WINDOW_AUTOSIZE ); // Create a window
    //cv::createTrackbar( "Min Threshold:", window_name, &lowThreshold, max_lowThreshold, CannyThreshold ); // A Trackbar to enter threshold
    char name[64];
    for( int i=0; i<=100; i+=10 ) {
        sprintf( name, "gom_canny_thresh=%d.jpg", i );
        CannyThreshold(i, 0); // Show the image
        cv::imwrite( name, final );
    }

    cv::waitKey(0); // Wait until user exit program by pressing a key


    return 0;
}
