#ifndef _UTIL_H_
#define _UTIL_H_

#include <cmath>
#include "typedefs.h"


//----------------------------------------------------------------------
class Util {
public:
    float getRand();
	VEC getRands(int n);
	float l2Norm(const VEC& vec);
	void printv(const VEC& v, char* msg, int mn=0, int mx=-1);
    void seedRandom();
    void show_mat(const MAT& input, const char* name, bool wait=true);
    void show_img(const MAT& input, const char* name, bool wait=true);
    void save_mat(const MAT& input, const char* name);
    MAT load_img( const std::string& filename );

    void wait();
	
};
//----------------------------------------------------------------------

#endif
