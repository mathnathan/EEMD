#include <opencv2/opencv.hpp>
#include <stdlib.h>
#include <armadillo>
#include <time.h>
#include "tools.h"
#include <vector>

const bool VERBOSE = 0;

//----------------------------------------------------------------------
float Tools::getRand() {
if(VERBOSE) printf("\nfloat Tools::getRand()\n");
    seedRandom();
	float f = (float)rand();

    return f;
}
//----------------------------------------------------------------------
VEC Tools::getRands(int n) {
if(VERBOSE) printf("\nVEC Tools::getRands(int n=%d)\n", n);
    VEC rands;
    rands.set_size(n);
    arma::Col<float>::iterator elem = rands.begin();
	float f;
    seedRandom();

	for (int i=0; i < n; i++) {
		f = (float)rand() / RAND_MAX;
		f = 2.0*f - 1.0;
		*elem = f;
        elem++;
	}

	return rands;
}
//----------------------------------------------------------------------
float Tools::l2Norm(const VEC& v) {
if(VERBOSE) printf("\nfloat Tools::l2Norm(const VEC& v)\n");

    float norm = arma::norm(v,2);
	return(norm);
}
//----------------------------------------------------------------------
void Tools::printv(const VEC& v, char* msg = "", int mn, int mx) {
if(VERBOSE) printf("\nvoid Tools::printv(const VEC& v, char* msg=%s, int mn=%d, int mx=%d)\n",msg,mn,mx);

    int length = v.n_elem;
	printf("length = %d\n", length);
	if (mx == 0) {
		mx = length;
	}
	for (int i=mn; i < mx; i++) {
		printf("%s: elem[%d] = %f\n", msg, i, v[i]);
	}
}
//----------------------------------------------------------------------
void Tools::seedRandom()  {
if(VERBOSE) printf("\nvoid Tools::seedRandom()\n");

	srand( time(NULL) );
}
//----------------------------------------------------------------------
void Tools::show_mat(const MAT& input, const char* name, bool wait)  {
if(VERBOSE) printf("\nvoid Tools::show_mat(const MAT& input, const char* name=%s, bool wait=%d)\n",name,wait);

    char winName[50];

    // Scale the input matrix to between 0 and 1 (OpenCV Requirement)
    float min = input.min();
    float max = input.max();
    printf("min = %f\n", min);
    printf("max = %f\n", max);

    MAT trans_input = input-min; // Translate to 0
    float scale = 1.0/(max-min);
    MAT scaled_input = trans_input*scale; // Scale between 0, 1

    //cv::Mat img(input.n_rows,input.n_cols,CV_32FC1,(void*)scaled_input.memptr());
    cv::Mat img(input.n_cols,input.n_rows,CV_32FC1,(void*)scaled_input.memptr());
    if( img.empty() ) {
        printf( "Image %s has no data!\n", name );
        exit(EXIT_FAILURE);
    }
    sprintf( winName, "MAT %s", name );
    int winFlag = CV_WINDOW_NORMAL;
    cv::namedWindow( winName, winFlag );
    
// Only to test data... --------
#if 1
    printf("depth = %d\n", img.depth() );
    printf("channels = %d\n", img.channels() );
    printf("0 |");
    for(int y=0; y<img.rows; y++ ) {
        if(y!=0) {
            printf("%d |", y);
        }
        for( int x=0; x<img.cols; x++ ) {
            printf(" %f ", img.at<float>(y,x)*(max-1)+min);
        }
        printf("|\n");
    }
#endif
// -----------------------------


    cv::imshow( winName, img );
    if(wait) {
        cv::waitKey(0);
    }
}
//----------------------------------------------------------------------
void Tools::save_mat(const MAT& input, const char* name)  {
if(VERBOSE) printf("\nvoid Tools::save_mat(const MAT& input, const char* name=%s)\n",name);

    // Scale the input matrix to between 0 and 255
    float min = input.min();
    float max = input.max();
    float scale = 255.0/(max-min);
    MAT scaled_input = input-min; // Translate to 0
    scaled_input = scaled_input*scale; // Scale between 0, 255

    cv::Mat img(scaled_input.n_rows,scaled_input.n_cols,CV_32FC1,(void*)scaled_input.memptr());
    cv::imwrite( name, img );
}
//----------------------------------------------------------------------
void Tools::wait() {
if(VERBOSE) printf("\nvoid Tools::wait()\n");

    cv::waitKey(0);
}
//----------------------------------------------------------------------
MAT Tools::load_img( const std::string& filename ) {
if(VERBOSE) printf("\nMAT Tools::load_img( const std::string& filename=%s )\n",filename.c_str());
    
    // Load in image and convert it to grayscale (must always be grayscale)
    cv::Mat input_image = cv::imread( filename, CV_LOAD_IMAGE_GRAYSCALE );
    if( input_image.empty() ) {
        printf( "\nERROR: Could no load image from file \"%s\".\n", filename.c_str() );
        exit(1);
    }
    cv::Mat tmp;
    if( input_image.type() != CV_32FC1 ) {
        input_image.convertTo(tmp, CV_32FC1);
    }

    float* data = (float*)tmp.data; // Pointer to OpenCV Mat data
    int rows = tmp.rows;
    int cols = tmp.cols;
    fmat arma_signal( data, rows, cols ); // Copies memory by default

    return arma_signal;

}
//----------------------------------------------------------------------
std::vector<std::string>& Tools::split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while(std::getline(ss, item, delim)) {
        elems.push_back(item);
    } 
    
    return elems;
}
//----------------------------------------------------------------------
std::vector<std::string> Tools::split(const std::string &s, char delim) {
    std::vector<std::string> elems;

    return split(s, delim, elems);
}
