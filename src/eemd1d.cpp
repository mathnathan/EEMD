#include "eemd1d.h"
#include "spline_class.h"
#include "util.h"
#include <vector>
using namespace std;

Util u;

//----------------------------------------------------------------------
VEC EEMD1D::addNoise(const VEC& signal, const double level)
{
	int n = signal.size();
	VEC rands = u.getRands(n);
	VEC new_signal(n);

	new_signal = signal + level*rands;
	return(new_signal);
}
//----------------------------------------------------------------------
VEC EEMD1D::one_iteration(const VEC& rands, int& nb_extremas)
{
	// find min/max of signal
	int n = rands.n_rows;
	vector<double> imin, imax; 
	vector<double> vmin, vmax;

	// first point, to be filled in later
	int count = 0;
	imax.push_back(0);
	imin.push_back(0);
	vmax.push_back(-1.);
	vmin.push_back(-1.);

	for (int i=2; i < n-1; i++) {
		// > or >=? 
		if (rands[i] > rands[i-1] && rands[i] > rands[i+1]) {
			imax.push_back(i);
			vmax.push_back(rands(i));
		}
		if (rands[i] < rands[i-1] && rands[i] < rands[i+1]) {
			imin.push_back(i);
			vmin.push_back(rands(i));
		}
	}

	// last point to be filled in later
	imax.push_back(n-1);
	imin.push_back(n-1);
	vmax.push_back(0.);
	vmin.push_back(0.);

	nb_extremas = imax.size();
	if (imin.size() < nb_extremas) {
		nb_extremas = imin.size();
	}

	if (nb_extremas < 3) return(vmin); // do not use return statement

	//-----------------------------
	// update the first and last min/max of function

	// left
	int imn1  = imin[1]; double vmn1 = rands[imn1];
	int imn2  = imin[2]; double vmn2 = rands[imn2];
	int imnn2 = imin[imin.size()-3]; double vmnn2 = rands[imnn2];
	int imnn1 = imin[imin.size()-2]; double vmnn1 = rands[imnn1];

	int imx1 = imax[1]; double vmx1 = rands[imx1];
	int imx2 = imax[2]; double vmx2 = rands[imx2];
	int imxn2 = imax[imax.size()-3]; double vmxn2 = rands[imxn2];
	int imxn1 = imax[imax.size()-2]; double vmxn1 = rands[imxn1];

	double vmn_left = vmn1 + (vmn2-vmn1)/(imn2-imn1) * (0-imn1);
	double vmx_left = vmx1 + (vmx2-vmx1)/(imx2-imx1) * (0-imx1);

	double vmn_right = vmnn1 + (vmnn2-vmnn1)/(imnn2-imnn1) * (n-1-imnn1);
	double vmx_right = vmxn1 + (vmxn2-vmxn1)/(imxn2-imxn1) * (n-1-imxn1);

	double left_func_value  = rands[0];
	double right_func_value = rands[n-1];

	// determine value of left and right extrema based on comparison between linear interp and actual value
	if (vmn_left  > left_func_value)  vmn_left  = left_func_value;
	if (vmn_right > right_func_value) vmn_right = right_func_value;
	if (vmx_left  < left_func_value)  vmx_left  = left_func_value;
	if (vmx_right < right_func_value) vmx_right = right_func_value;

	vmin[0] = vmn_left;
	vmax[0] = vmx_left;
	vmin[imin.size()-1] = vmn_right;
	vmax[imax.size()-1] = vmx_right;

//-----------------------------
	// fit a cubic spline through the minima and through the maxima. 
	// find a library
	Spline spline_min(imin, vmin);
	Spline spline_max(imax, vmax);
	spline_min.spline(0,0,0.,0.); // natural BC  (ISSUE?)
	spline_max.spline(0,0,0.,0.);

	//----------------

	VEC rands_new(n);
	VEC v_min(n), v_max(n), diff(n);

	for (int i=0; i < n; i++) {
		v_min[i] = spline_min.eval(i);
		v_max[i] = spline_max.eval(i);
	}

	diff = 0.5*(v_max + v_min);
	rands_new = rands - diff;

	return(diff);
}
//----------------------------------------------------------------------
VEC EEMD1D::compute_imf(const VEC& rands, int& nb_extremas)
{
	VEC rands_new = rands;

	for (int i=0; i < 7; i++) {
		VEC diff = one_iteration(rands_new, nb_extremas);
		if (nb_extremas < 3) break; // do not use return value

		double diff_norm = u.l2Norm(diff);
		double rands_norm = u.l2Norm(rands_new);

		rands_new = rands_new - diff;

		if (diff_norm / rands_norm < .05) break;
	}
	return(rands_new);
}
//----------------------------------------------------------------------
vector<VEC> EEMD1D::compute_imfs(const VEC& signal_orig)
{
	int n = signal_orig.size();

	// compute eem of noisy signal

	vector<VEC> imfs;
	VEC imf;
	VEC rands;

	rands = signal_orig;
	residual = rands;

	for (int i=0; i < 8; i++) {
		int nb_extremas = -1;
		// 1st argument is a copy , so cannot be changed
		imf = compute_imf(residual, nb_extremas);
		if (nb_extremas < 3) break; // returned imf not valid
		imfs.push_back(imf);

		residual = residual - imf;
	}

	return imfs;
}
//----------------------------------------------------------------------
vector<VEC> EEMD1D::compute_noise_imfs(const VEC& noiseless_signal, double level, int max_it)
{
	// Loop over noise
	int sz;
	VEC signals;

	// first call
	int nb_calls = 1;
	signals = addNoise(noiseless_signal, level);
	vector<VEC> imfs = compute_imfs(signals);
	sz = imfs.size();

	// Set max number of loops to nonzero if noise method
	for (int i=0; i < max_it; i++) {
		nb_calls++;
		signals = addNoise(noiseless_signal, level);
		vector<VEC> imfs1 = compute_imfs(signals);

		sz = imfs.size() < imfs1.size() ? imfs.size() : imfs1.size();

		for (int i=0; i < sz; i++) {
			VEC& imf = imfs[i];
			imf = imf + imfs1[i];
		}
	}

	double dc = (double) nb_calls;

	for (int i=0; i < sz; i++) {
		VEC& imf = imfs[i];
		imf = imf / dc;
	}

	return(imfs);
}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
