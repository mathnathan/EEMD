#ifndef _MEEMD_H_
#define _MEEMD_H_

#include <vector>
#include "eemd1d.h"
#include "typedefs.h"

class MEEMD
{
public:

    // MAT is a typedef for OpenCV's Mat data structure.
	MAT signal;

    std::vector<VEC> xDecomp;
    std::vector<VEC> yDecomp;

    // Currently only supports loading a 2D image. Should make robust to allow
    // any data to be loaded, and to perform either 1 or 2 dimensional EEMD
    // depending on the input data.
    bool load( const string& filename );

    // Many future possibilities for combinig options between OpenCV's displays
    // and those of matplotpp. For now MEEMD::show() only displays the Image
    // loaded from the MEEMD::load() routine.
    void show();

    // This will decompose the 2D image in either the x or y direction
    void decompose( MAT& signal, double level, int dimension );

    // This will combine the decompositions back into the IMF 
    MAT combine(); 

};

#endif
