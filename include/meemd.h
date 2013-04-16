#ifndef _MEEMD_H_
#define _MEEMD_H_

#include <vector>
#include "eemd.h"
#include "typedefs.h"
//#include "tools.h"

// Types of data that can be read
const int MEEMD_NETCDF = 0;
const int MEEMD_IMAGE = 1;

class Tools;

class MEEMD
{
public:

    // Use the EEMD class to decompose the input signal
    EEMD* eemd;

    // Use tools in the MEEMD implementation
    Tools* tools;

    // MAT is a typedef for armadillo's mat data structure
    // The original input signal
	MAT signal;

    // Used in the decomposition process. Hold onto if 
    // needed for any potential post processing
    std::vector< std::vector<MAT> > h;

    // Stores the final multidimensional decomposition
    std::vector<MAT> final_imfs;

    // Constructor for initializing the MEEMD class
    MEEMD( const std::string& filename, int type );
    MEEMD();
    ~MEEMD();

    bool load( const std::string& filename, int );
    bool save( const MAT& input, const std::string& filename );
    bool save( const std::string& dir="./" );
    void show();
    void showimf( int );
    void decompose( float, int, int=-1 );
    std::vector<MAT> decomposeRows( const MAT&, float, int, int );
    std::vector<MAT> decomposeCols( const MAT&, float, int, int );
    std::vector<MAT> combine(); 
};

#endif
