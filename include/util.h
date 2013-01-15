#ifndef _UTIL_H_
#define _UTIL_H_

#include <cmath>
#include <vector>
#include "typedefs.h"


//----------------------------------------------------------------------
class Util {
public:
	std::vector<double> getRands(int n);
	double l2Norm(const VEC& vec);
	void printv(const VEC& v, char* msg, int mn=0, int mx=-1);
    void seedRandom();
	
};
//----------------------------------------------------------------------

#endif
