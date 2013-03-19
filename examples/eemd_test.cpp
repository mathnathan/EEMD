#include <vector>
#include <stdlib.h>
#include <stdio.h>

#include "matplotpp.h"
#include "typedefs.h"
#include "util.h"
#include "eemd.h"

using namespace std;

VEC createSignal(int n);
VEC noiseless_signal;
float thresh = 0.05;
MAT imfs;

//void plot(VEC& a, VEC& b);
//void plot(std::vector<double>& a, std::vector<float>& b);

EEMD* eemdptr;

#include <fstream>
#include <iostream>
#include <cstdlib>


//----------------------------------------------------------------------
VEC read_stock_data()
{
// return closing prices

	VEC close( 10000 );
	char date[16];
	float o, h, l, c, v, dum;
	int nbc;
	string line;

	ifstream fd;
	fd.open("../data/ung.txt", ifstream::in);
    if(!fd) { // File could not be opened
        cerr << "Error: file could not be opened\n";
        exit(1);
    }

    arma::Col<float>::iterator elem = close.begin();
    while( !fd.eof() ) {
		getline(fd, line);
		if (line.size() == 0) break;
		nbc = sscanf(line.c_str(), "%s %f %f %f %f %f %f", date, &o, &h, &l, &c, &v, &dum);
	    *elem = (float)c;
        elem++;
	}

    fd.close();

	return( close );
}
//----------------------------------------------------------------------
VEC createSignal( size_t n )
{
    MAT test(3,3);
    for( int x=0; x<3; x++ ) {
        for( int y=0; y<3; y++ ) {
            test(x,y) = 2*x+y;
        }
    }
    MAT test_trans = test.t();

    // Only to test data... --------
#if 1
        printf("0 |");
       for(int y=0; y<3; y++ ) {
           if(y!=0) {
           printf("%d |", y);
           }
           for( int x=0; x<3; x++ ) {
               printf(" %f ", test(x,y));
           }
            printf("|\n");
       }
#endif
    // -----------------------------

    // Only to test data... --------
#if 1
        printf("\n\n0 |");
       for(int y=0; y<3; y++ ) {
           if(y!=0) {
           printf("%d |", y);
           }
           for( int x=0; x<3; x++ ) {
               printf(" %f ", test_trans(x,y));
           }
            printf("|\n");
       }
#endif
    // -----------------------------


	//return(read_stock_data());
	float pi = 3.14159;
    
	#if 0 // Plain sine wave
	VEC signal; signal.resize(n);
    arma::Col<float>::iterator elem = signal.begin();

    float step = (9*pi)/n;
    for( int i=0; i<n; i++ ) {
        *elem = sin(i*step);
        elem++;
    }

	#elif 0 // Sum of two waves
    VEC cosine1; cosine1.resize(n);
    VEC cosine2; cosine2.resize(n);
    arma::Col<float>::iterator elem1 = cosine1.begin();
    arma::Col<float>::iterator elem2 = cosine2.begin();

    float step = (4*pi)/n;
    for( int i=0; i<n; i++ ) {
        *elem1 = cos(i*step);
        *elem2 = cos(2*i*step);
        elem1++;
        elem2++;
    }
    
    VEC signal = cosine1 + cosine2;
    #else
    float indata[32] = {130.000000, 125.000000,  126.000000,  129.000000,  129.000000,  128.000000, 126.000000,  128.000000,  133.000000,  135.000000,  133.000000,  135.000000, 134.000000,  135.000000,  135.000000,  137.000000,  138.000000,  141.000000, 142.000000,  138.000000,  137.000000,  135.000000,  137.000000,  140.000000, 137.000000,  136.000000,  133.000000,  136.000000,  138.000000,  136.000000, 132.000000,  130.000000 };
	VEC signal; signal.resize(32);
    arma::Col<float>::iterator elem = signal.begin();
    for( int i=0; i<32; i++ ) {
        *elem = indata[i];
        elem++;
    }
    #endif
	return(signal);
}
//----------------------------------------------------------------------
class MP : public MatPlot{ 

    void call_plot( std::vector<double> ix, VEC& y ) {

        std::vector<double> iy = conv_to< vector<double> >::from(y);
        plot(ix, iy);
    }

    void DISPLAY() {

        int num_imfs = imfs.n_cols;
        int nb = imfs.n_rows;

        std::vector<double> ix;

        for (int i=0; i < nb; i++) {
            ix.push_back(i);
        }

        subplot(3,3,1);
        title("original signal");
        //axis( 0,1000,-1,1 );
        call_plot(ix,eemdptr->input_signal);

        char titlee[10];
        for (int i=0; i < imfs.n_cols; i++) {
            sprintf(titlee, "imf %d", i+1);
            subplot(3,3,i+2);
            title(titlee);
            VEC imf = imfs.col(i);
            //if( i<4 ) axis( 0,1000,-0.2,0.2 );
            //else if( i>4 ) axis( 0,1000,-.1,.1 );
            //else axis( 0,1000,-1,1 );
            call_plot( ix, imf );
        }

        subplot(3,3,num_imfs+2);
        title("residual");
        //axis( 0,1000,-1,1 );
        call_plot(ix,eemdptr->residual);
    }
}mp;
void display(){ mp.display(); }
void reshape(int w,int h){ mp.reshape(w,h); }
//----------------------------------------------------------------------
int main(int argc,char* argv[])
{
    size_t test_size = 1000;
	noiseless_signal = createSignal( test_size );
	eemdptr = new EEMD( noiseless_signal );
    //noiseless_signal = eemdptr->addNoise( noiseless_signal, 0.1 );
    //eemdptr->addNoise( 0.2 );

    float noise_amplitude = 0.1;
    int nb_imfs = 6;
	int nb_noise_iters = 100;

    ROW input_row( (eemdptr->input_signal).t() );
    printf("Input signal\n");
    (eemdptr->input_signal).print();
    printf("Row signal\n");
    input_row.print();
    printf("input_row.size() = %ld\n", input_row.size() );
	imfs = eemdptr->eemdf90( input_row, noise_amplitude, nb_imfs, nb_noise_iters );
    imfs = imfs.t();

    glutInit(&argc, argv);
    glutCreateWindow(100,100,400,300);
    glutDisplayFunc( display );
    glutReshapeFunc( reshape );
    glutMainLoop();    
	return(0);
}
//----------------------------------------------------------------------
