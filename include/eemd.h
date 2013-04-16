#ifndef _EEMD_H_
#define _EEMD_H_

#include <vector>
#include "typedefs.h"
//#include "tools.h"

// Fortran eemd implementation see eemdf90/eemd.f90 for details
extern "C" {
    void eemd_( int&, float*, float&, int&, int&, int&, float*);
}

class Tools;

class EEMD
{
public:
    // Use tools in the MEEMD implementation
    Tools* tools;

    VEC input_signal;
    MAT decomposition;
	VEC residual;
    int signal_length;

    // Constructors
    EEMD( const std::string& fileName );
    EEMD( int n );
    EEMD( const VEC& input_array );
    EEMD( );

    // Destructor
    ~EEMD();

    bool load( const std::string& filename );

    // Not using the C++ eemd implementation for now...
    /*
	VEC one_iteration(const VEC& rands, int& nb_extremas);
	VEC compute_imf(const VEC& rands, int& nb_extremas);
	MAT compute_imfs(const VEC& signal_orig);;
	MAT compute_noise_imfs(const VEC& noiseless_signal, float level, int max_it);
	MAT compute_noise_imfs(const ROW& noiseless_signal, float level, int max_it);
    */
    void addNoise(const float level);
	VEC addNoise(const VEC& signal, const float level);

    // Use the F90 eemd implementation until have time to debug c++ code
    MAT eemdf90( float noise_amplitude, int num_imfs, int num_ensembles );
    MAT eemdf90( VEC input, float noise_amplitude, int num_imfs, int num_ensembles );
    MAT eemdf90( ROW input, float noise_amplitude, int num_imfs, int num_ensembles );

};
#endif
