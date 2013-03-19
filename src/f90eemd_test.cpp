#include <vector>
#include <stdlib.h>
#include <stdio.h>

#include "matplotpp.h"
#include "typedefs.h"
#include "util.h"
#include "eemd.h"

using namespace std;

VEC createSignal(int n);
VEC input_data;
double thresh = 0.05;
MAT imfs;

void plot(VEC& a, VEC& b);

EEMD* eemd;

#include <fstream>
#include <iostream>
#include <cstdlib>


//----------------------------------------------------------------------
VEC read_stock_data()
{
// return closing prices

	vector<double> close;
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

    while( !fd.eof() ) {
		getline(fd, line);
		if (line.size() == 0) break;
		nbc = sscanf(line.c_str(), "%s %f %f %f %f %f %f", date, &o, &h, &l, &c, &v, &dum);
		close.push_back(c);
	}

    fd.close();

	return(VEC(close));
}
//----------------------------------------------------------------------
VEC createSignal(int n)
{
	//return(read_stock_data());

	Util u;
	double del = 3.14*2 / 20;
	input_data.resize(n);

	#if 0
	for (int i=0; i < n; i++) {
		input_data[i] = sin(del*i) + 5.*cos(5*del*i + 9*del*i) + 13.1*cos(3.3*del*i); // + 0.05*rands[i];
	}
	return(input_data);
	#endif

	double pi = 3.14159;
	int sz = (1 << 11) - 20;
	n = 1 << 11;

	#if 1
	input_data.resize(sz);
	int count = 0;
	for (int t=20; t < n; t++) {
		double e1 = pow(t/80., 1.5);
		double e2 = pow(t/100., 0.8);
		//printf("e1,e2= %f, %f\n", e1, e2);
		input_data[t-20] = 10.*cos(e1*pi)+2.*cos(e2*pi)+ (t/500.+2.);
		count++;

		//input_data[t-20] = 10.*cos((t/70.)*pi) + 0.*5.*cos((t/30.)*pi);
		// reverse signal
		//input_data[n-1-t+20] = 10.*cos((t/70.)*pi) + 1.*5.*cos((t/30.)*pi);
	}
	printf("count= %d, size= %d\n", count, input_data.size());
	return(input_data);
	#endif

	#if 0
	input_data.resize(sz);
	for (int t=20; t < n; t++) {
		double e1 = pow(t/100., 1.8);
		double e2 = pow(t/200., 1.5);
		printf("%f, %f\n", e1, e2);
		input_data[t-20] = 5.*cos(e1*pi)+e2*cos(t*pi/180.)+ (t/200.-5.)*(t/200.-5.);
	}
		//printf("%f\n", input_data[0]);
	return(input_data);
	#endif

}

int main() {

    int timeSeriesLength=1000, emembers=100, num_imfs=12;
    double input_data, eemd_result;
    double stdev=0.2;

    input_data = createSignal( timeSeriesLength );
    eemd = new EEMD();
    
    double* input_ptr = input_data.t().memptr();
    eemd_(timeSeriesLength,input_ptr,stdev,emembers,num_imfs,idum,eemd_result);
    return 0;
}
