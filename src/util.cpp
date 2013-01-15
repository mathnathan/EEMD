#include <stdlib.h>
#include "util.h"

#include <vector>
using namespace std;

//----------------------------------------------------------------------
std::vector<double> Util::getRands(int n)
{
	vector<double> rands(n);
	double f;

    seedRandom();

	for (int i=0; i < n; i++) {
		f = (double) rand() / RAND_MAX;
		f = 2.0*f - 1.0;
		//rands[i] = sf_rand(-1.,1.);
		rands[i] = f;
	}

	return rands;
}
//----------------------------------------------------------------------
double Util::l2Norm(const VEC& v)
{
	//return(norm(v,2));
	//double norm = 0.;
	//int n = vec.size();

	double norm = sqrt(arma::dot(mat(v),mat(v)));
	//for (int i=0; i < n; i++) {
		//norm += vec[i]*vec[i];
	//}
	//norm = sqrt(norm);
	return(norm);
}
//----------------------------------------------------------------------
void Util::printv(const VEC& v, char* msg = "", int mn, int mx)
{
	printf("inside printv\n");
	printf("%d\n", v.size());
	if (mx == 0) {
		mx = v.size();
	}
	for (int i=mn; i < mx; i++) {
		printf("%s, %d, %f\n", msg, i, v[i]);
	}
}
//----------------------------------------------------------------------
void Util::seedRandom() 
{
#ifdef __MAC__
    sranddev();
#else
    #include <time.h>
	srand( time(NULL) );
#endif
}
//----------------------------------------------------------------------
