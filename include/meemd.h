#ifndef _MEEMD_H_
#define _MEEMD_H_

#include <vector>
#include "typedefs.h"

// Types of data that can be read
const int MEEMD_NETCDF = 0;
const int MEEMD_IMAGE = 1;

class Tools;
class EEMD;

class MEEMD
{
public:
    // Constructor for initializing the MEEMD class
    MEEMD( const std::string& filename, int type );
    MEEMD();
    ~MEEMD();

    // I/O Routines
    bool load( const std::string& filename, int type );
    bool save( const MAT& input, const std::string& filename );
    bool save( const std::string& dir="./" );
    void showInput();
    void showIMF( int );
    void showOutput();

    // Decomposition Routines
    void decompose( float, int, int=-1, int=-1 );
    std::vector<MAT> decomposeRows( const MAT&, float, int, int );
    std::vector<MAT> decomposeCols( const MAT&, float, int, int );
    std::vector<MAT> combine(); 

    // CV routines
 
private:
    // Use the EEMD class to decompose the input signal
    EEMD* eemd;

    // Use tools (OpenCV) in the MEEMD implementation
    Tools* tools;

    // MAT is a typedef for armadillo's mat data structure
    // The original input signal
	MAT signal;

    // Used in the decomposition process. Hold onto if 
    // needed for any potential post processing
    std::vector< std::vector<MAT> > h;

    // Stores the final multidimensional decomposition
    std::vector<MAT> final_imfs;

    // This will store the type of data the has been decomposed
    // with this specific instance
    int dtype;

   
};

#endif
