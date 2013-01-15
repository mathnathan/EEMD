
#ifndef _EEMD1D_H_
#define _EEMD1D_H_

#include <vector>
#include "typedefs.h"

class EEMD1D
{
public:
	VEC residual;

	VEC addNoise(const VEC& signal, const double level);
	VEC one_iteration(const VEC& rands, int& nb_extremas);
	VEC compute_imf(const VEC& rands, int& nb_extremas);
	std::vector<VEC> compute_imfs(const VEC& signal_orig);;
	std::vector<VEC> compute_noise_imfs(const VEC& noiseless_signal, double level, int max_it);
};
#endif
