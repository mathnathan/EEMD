//
#include <vector>
#include <stdlib.h>
#include <stdio.h>

#include "matplotpp.h"
#include "typedefs.h"
#include "util.h"
#include "eemd1d.h"

using namespace std;


VEC createSignal(int n);
VEC noiseless_signal;
double thresh = 0.05;
vector<VEC> imfs;

void plot(VEC& a, VEC& b);

EEMD1D* eemd;

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
	fd.open("../ung.txt", ifstream::in);
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

	return(close);
}
//----------------------------------------------------------------------
VEC createSignal(int n)
{
	return(read_stock_data());

	Util u;
	double del = 3.14*2 / 20;
	noiseless_signal.resize(n);

	#if 0
	for (int i=0; i < n; i++) {
		noiseless_signal[i] = sin(del*i) + 5.*cos(5*del*i + 9*del*i) + 13.1*cos(3.3*del*i); // + 0.05*rands[i];
	}
	return(noiseless_signal);
	#endif

	double pi = 3.14159;
	int sz = (1 << 11) - 20;
	n = 1 << 11;

	#if 1
	noiseless_signal.resize(sz);
	int count = 0;
	for (int t=20; t < n; t++) {
		double e1 = pow(t/80., 1.5);
		double e2 = pow(t/100., 0.8);
		//printf("e1,e2= %f, %f\n", e1, e2);
		noiseless_signal[t-20] = 10.*cos(e1*pi)+2.*cos(e2*pi)+ (t/500.+2.);
		count++;

		//noiseless_signal[t-20] = 10.*cos((t/70.)*pi) + 0.*5.*cos((t/30.)*pi);
		// reverse signal
		//noiseless_signal[n-1-t+20] = 10.*cos((t/70.)*pi) + 1.*5.*cos((t/30.)*pi);
	}
	printf("count= %d, size= %d\n", count, noiseless_signal.size());
	return(noiseless_signal);
	#endif

	#if 0
	noiseless_signal.resize(sz);
	for (int t=20; t < n; t++) {
		double e1 = pow(t/100., 1.8);
		double e2 = pow(t/200., 1.5);
		printf("%f, %f\n", e1, e2);
		noiseless_signal[t-20] = 5.*cos(e1*pi)+e2*cos(t*pi/180.)+ (t/200.-5.)*(t/200.-5.);
	}
		//printf("%f\n", noiseless_signal[0]);
	return(noiseless_signal);
	#endif

}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
class MP : public MatPlot{ 

void plot1(VEC& a, VEC& b)
{
	std::vector<double> aa = conv_to< vector<double> >::from(a);
	std::vector<double> bb = conv_to< vector<double> >::from(b);
	plot(aa, bb);
}

void DISPLAY()
{
	int which_imf = 0;
	int sz = imfs.size();

	int nb(noiseless_signal.size());
	//vector<double> ix(nb);
	VEC ix; ix.set_size(nb);

	for (int i=0; i < nb; i++) {
		ix[i] = i;
	}

#if 1
	// sum all the imfs
	vector<double> sum(nb);
	for (int i=0; i < imfs.size(); i++) {
		VEC& imf = imfs[i];
		for (int j=0; j < nb; j++) {
			sum[j] += imf[j];
		}
	}
	for (int j=0; j < nb; j++) {
		sum[j] += eemd->residual[j];
	}

	if (which_imf > sz-1) {
		//printf("which_imf reset to imfs.size()-1\n");
		which_imf = imfs.size()-1;
	}

	VEC& imf = imfs[which_imf];

	char titlee[80];
	for (int i=0; i < imfs.size(); i++) {
		sprintf(titlee, "imf %d", i);
		subplot(3,3,i+1);
		title(titlee);
		plot1(ix,imfs[i]);
	}

	#if 1
	subplot(3,3,imfs.size()+1);
	title("original signal");
	plot1(ix,noiseless_signal);
	subplot(3,3,imfs.size()+2);
	title("residual");
	plot1(ix,eemd->residual);
	#endif
#endif
}
}mp;
void display(){ mp.display(); }
void reshape(int w,int h){ mp.reshape(w,h); }
//----------------------------------------------------------------------
int main(int argc,char* argv[])
{
	eemd = new EEMD1D();
	noiseless_signal = createSignal(1000);

	int nb_noise_iter = 100;
	double noise_level = 1;
	if (nb_noise_iter == 0) noise_level = 0.0; // should be wrt signal, TODO

	imfs = eemd->compute_noise_imfs(noiseless_signal, noise_level, nb_noise_iter);

	//compute();
    glutInit(&argc, argv);
    glutCreateWindow(100,100,400,300);
    glutDisplayFunc( display );
    glutReshapeFunc( reshape );
    glutMainLoop();    
	return(0);
}
//----------------------------------------------------------------------
