#ifndef _MEEMD_H_
#define _MEEMD_H_

#include <vector>
#include "eemd1d.h"
#include "typedefs.h"

class MEEMD
{
public:

	MAT signal;

    bool load( const string& filename );
    void show();
    VEC decompose( MAT& signal, double level, int dimension );
    MAT combine(); 

};

#endif
