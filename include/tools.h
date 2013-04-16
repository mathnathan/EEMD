#ifndef _TOOLS_H_
#define _TOOLS_H_

#include <cmath>
#include "typedefs.h"

class MEEMD;

//----------------------------------------------------------------------
class Tools {
public:
    float getRand();
	VEC getRands(int n);
	float l2Norm(const VEC& vec);
	void printv(const VEC& v, char* msg, int mn=0, int mx=-1);
    void seedRandom();
    void show_mat(const MAT& input, const char* name, bool wait=true);
    void save_mat(const MAT& input, const char* name);
    MAT load_img( const std::string& filename );
    std::vector<std::string> &split(const std::string &s, char delim,
                                    std::vector<std::string> &elems);
    std::vector<std::string> split(const std::string &s, char delim);
    void wait();
	
};
//----------------------------------------------------------------------

#endif
