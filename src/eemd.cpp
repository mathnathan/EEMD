/*
 * Member variables:
 *      VEC input_signal;
 *      VEC decomposition;
 *      VEC residual;
 *      size_t signal_length;
 */      

#include "eemd.h"
#include "spline_class.h"
#include "typedefs.h"
#include <vector>
#include <stdio.h>
#include "tools.h"

//----------------------------------------------------------------------
EEMD::EEMD( const std::string& fileName ) {

    if(  !load( fileName ) ) {
        printf("Could not open or find the image\n");
        exit( EXIT_FAILURE );
    }

    tools = new Tools();

}   
//----------------------------------------------------------------------
EEMD::EEMD( int n ) {

    // Create a shell for the input signal and store it internally
    input_signal.zeros(n);
    signal_length = n;

    tools = new Tools();
}       
//----------------------------------------------------------------------
EEMD::EEMD( const VEC& input_array ) {

    signal_length = input_array.size();
    input_signal = input_array;

    tools = new Tools();
}   
//----------------------------------------------------------------------
EEMD::EEMD( ) {

    tools = new Tools();
}
//----------------------------------------------------------------------
EEMD::~EEMD( ) {

    delete tools;
}
//----------------------------------------------------------------------
bool EEMD::load( const std::string& fileName )
{   
    // read data and stuff
    // Doesn't do anything yet...
    return false;

    return true;
}
//----------------------------------------------------------------------
bool EEMD::zeros( int n ){
    input_signal.zeros(n);
    signal_length = n;
    return true;
}
//----------------------------------------------------------------------
void EEMD::addNoise(const float level)
{
    VEC rands = tools->getRands( signal_length );
    input_signal += level*rands;
}               
//----------------------------------------------------------------------
VEC EEMD::addNoise(const VEC& signal, const float level)
{
    int n = signal.size();
    VEC rands = tools->getRands(n);
    VEC new_signal(n);

    new_signal = signal + level*rands;
    return(new_signal);
}
//----------------------------------------------------------------------
VEC EEMD::get_input() {
    return input_signal;
}
//----------------------------------------------------------------------
VEC EEMD::get_residual(){
    return residual;
}
/*----------------------------------------------------------------------
VEC EEMD::one_iteration(const VEC& rands, int& nb_extremas)
{
    // find min/max of signal
    int n = rands.n_rows;
    std::vector<float> imin, imax; 
    std::vector<float> vmin, vmax;

    // for extrema spanning multiple elements
    float spanMax=0;
    float spanMin=0;
    int lcv;

    // first point, to be filled in later
    int count = 0;
    imax.push_back(0);
    imin.push_back(0);
    vmax.push_back(-1.);
    vmin.push_back(-1.);


    // This method handles the nonunique extrema situation (repeated extrema)
    // which is much more common when decomposing images
    for ( lcv=2; lcv < n-1; lcv++) {
        // Initiate extrema locator by finding the signal's initial slope
        if ( rands[lcv-1] < rands[lcv] ) { 
            spanMax=1; // indicate the signal is increasing
            lcv++;
            break;
        }   
        else if ( rands[lcv-1] > rands[lcv] ) { 
            spanMin=1; // indicate the signal is decreasing
            lcv++;
            break;
        }   
        else // Points are equal
            continue; // Keep iterating. Must have nonzero slope to proceed
    }   

    for ( lcv; lcv < n-1; lcv++) {
        if ( spanMax ) { // was increasing
            if ( rands[lcv-1] < rands[lcv] ) // still increasing
                spanMax = 1; // reset spanMax in case there was a span (saddle point)
            else if ( rands[lcv-1] > rands[lcv] ) { // found max
                spanMax++; // Moves us back one index to the actual extrema. *next line
                imax.push_back( (float)lcv-spanMax/2.0 );
                vmax.push_back( rands[lcv-1] );
                spanMax=0; // no longer increasing
                spanMin=1; // now decreasing
            }       
            else // if not greater or less must be equal
                spanMax += 1; // keep track of the span
        }           
        else if ( spanMin ) {// was decreasing
            if ( rands[lcv-1] > rands[lcv] ) // still decreasing
                spanMin = 1; // reset spanMin in case there was a sapn (saddle point)
            else if ( rands[lcv-1] < rands[lcv] ) { // found min
                spanMin++;
                imin.push_back( (float)lcv-spanMin/2.0 );
                vmin.push_back( rands[lcv-1] );
                spanMin=0; // no longer decreasing
                spanMax=1; // now increasing
            }       
            else // if not greater or less must be equal
                spanMin += 1; // keep track of the span
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
    float imn1  = imin[1]; float vmn1 = vmin[1];
    float imn2  = imin[2]; float vmn2 = vmin[2];
    float imnn2 = imin[imin.size()-3]; float vmnn2 = vmin[vmin.size()-3];
    float imnn1 = imin[imin.size()-2]; float vmnn1 = vmin[vmin.size()-2];

    float imx1 = imax[1]; float vmx1 = vmax[1];
    float imx2 = imax[2]; float vmx2 = vmax[2];
    float imxn2 = imax[imax.size()-3]; float vmxn2 = vmax[vmax.size()-3];
    float imxn1 = imax[imax.size()-2]; float vmxn1 = vmax[vmax.size()-2];

    float vmn_left = vmn1 + (vmn2-vmn1)/(imn2-imn1) * (0-imn1);
    float vmx_left = vmx1 + (vmx2-vmx1)/(imx2-imx1) * (0-imx1);

    float vmn_right = vmnn1 + (vmnn2-vmnn1)/(imnn2-imnn1) * (n-1-imnn1);
    float vmx_right = vmxn1 + (vmxn2-vmxn1)/(imxn2-imxn1) * (n-1-imxn1);

    float left_func_value  = rands[0];
    float right_func_value = rands[n-1];

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

    VEC v_min(n), v_max(n), diff(n);

    for (int i=0; i < n; i++) {
        v_min[i] = spline_min.eval(i);
        v_max[i] = spline_max.eval(i);
    }

    diff = 0.5*(v_max + v_min);

    return(diff);
}
//----------------------------------------------------------------------
VEC EEMD::compute_imf(const VEC& rands, int& nb_extremas)
{
    VEC rands_new = rands;

    for (int i=0; i < 7; i++) {
        VEC diff = one_iteration(rands_new, nb_extremas);
        if (nb_extremas < 3) break; // do not use return value

        float diff_norm = tools->l2Norm(diff);
        float rands_norm = tools->l2Norm(rands_new);

        rands_new = rands_new - diff;

        if (diff_norm / rands_norm < .05) break;
    }
    return(rands_new);
}
//----------------------------------------------------------------------
MAT EEMD::compute_imfs(const VEC& signal_orig)
{
    int n = signal_orig.size();
    int max_imfs = 3;

    // compute eem of noisy signal

    VEC imf;
    VEC rands;
    //MAT imfs = arma::zeros(n,max_imfs);
    MAT imfs;

    rands = signal_orig;
    residual = rands;

    int imf_cntr=0;
    for (int i=0; i < max_imfs; i++) {
        imf_cntr++;
        int nb_extremas = -1;
        // 1st argument is a copy , so cannot be changed
        imf = compute_imf(residual, nb_extremas);
        //printf("\tnb_extremas = %d\n", nb_extremas);
        if (nb_extremas < 3) break; // returned imf not valid
        imfs.insert_cols(i, imf);

        residual = residual - imf;
    }

    //printf("nb_imfs before = %d\n",nb_imfs);
    //printf("imf_cntr = %d\n",imf_cntr);
    //nb_imfs = imf_cntr < nb_imfs ? imf_cntr : nb_imfs;
    //printf("nb_imfs after = %d\n",nb_imfs);

    return imfs;
}
//----------------------------------------------------------------------
MAT EEMD::compute_noise_imfs(const VEC& noiseless_signal, float level, int max_it)
{
    // Loop over noise
    int sz;
    VEC signals;

    // first call
    int nb_calls = 1;
    signals = addNoise(noiseless_signal, level);
    MAT imfs = compute_imfs(signals);
    sz = imfs.n_cols;

    // Set max number of loops to nonzero if noise method
    for (int i=0; i < max_it-1; i++) {
        nb_calls++;
        signals = addNoise(noiseless_signal, level);
        MAT imfs1 = compute_imfs(signals);

        sz = imfs.n_cols < imfs1.n_cols ? imfs.n_cols : imfs1.n_cols;

        for (int i=0; i < sz; i++) {
            imfs.col(i) += imfs1.col(i);
        }
    }

    float dc = (float) nb_calls;

    for (int i=0; i < sz; i++) {
        imfs.col(i) /= dc;
    }

    //printf("\n\n NB_IMFS = %d \n\n", nb_imfs);
    //nb_imfs = std::numeric_limits<int>::max();

    return(imfs);
}
//----------------------------------------------------------------------
MAT EEMD::compute_noise_imfs(const ROW& noiseless_signal, float level, int max_it)
{
    VEC col = noiseless_signal.t();
    MAT imfs = compute_noise_imfs(col, level, max_it);
    return(imfs.t());
}
//----------------------------------------------------------------------
*/
//----------------------------------------------------------------------
MAT EEMD::eemdf90( VEC input, float noise_amplitude, int num_imfs, int num_ensembles ) {

    // Obtain a pointer to the data in the input array for the Fortran function
    float* indata = input.memptr();
    int length = input.size();
    int seed = tools->getRand(); // C++ random seed for the F90 random number generator
    float* result = new float[length*(num_imfs+2)]; // Store the results

    // Call the Fortran subroutine
    eemd_( length, indata, noise_amplitude, 
            num_ensembles, num_imfs, seed, result );

    // Create the resulting armadillo structure to store the result
    MAT imfs;
    imfs = arma::Mat<float>(result,length,num_imfs+2); 

    //printf("(rows,cols) = (%d,%d)\n", decomposition.n_rows, decomposition.n_cols);
    // The last column of the result is the residue (from F90 documentation)
    residual = imfs.col(num_imfs+1);
    imfs.shed_col(num_imfs+1);

    //printf("(rows,cols) = (%d,%d)\n", imfs.n_rows, imfs.n_cols);
    // The Very first column is the original singal (from F90 documentation)
    VEC f90_input = imfs.col(0);
    imfs.shed_col(0);

    //printf("(rows,cols) = (%d,%d)\n", imfs.n_rows, imfs.n_cols);
    // A sanity check to help see if the F90 result makes any sense
    float diff = norm( f90_input-input, 2 );
    if( diff > .1 && false ) {
        //printf( "\n\tFortran input signal differs from the C++ input signal by %f \n", diff);
        for( int i=0; i<length; i++ ) {
            printf( "C++[%d] = %f \t F90[%d] = %f\n", i, input[i], i, f90_input[i] );
        }
        exit(EXIT_FAILURE);

    }

    return imfs;

}
//----------------------------------------------------------------------
MAT EEMD::eemdf90( ROW input, float noise_amplitude, int num_imfs, int num_ensembles ) {

    // Tanspose the Row vector into a Column vector
    VEC col_input = input.t();
    MAT col_imfs = eemdf90(col_input, noise_amplitude, num_imfs, num_ensembles);
    MAT row_imfs = col_imfs.t();
    
    return row_imfs;

}
//----------------------------------------------------------------------
MAT EEMD::eemdf90( float noise_amplitude, int num_imfs, int num_ensembles ) {

    if( input_signal.is_empty() ) {
        printf( "No input signal to decompose. Load a signal with load().\n" );
        exit(EXIT_FAILURE);
    }

    decomposition = eemdf90(input_signal, noise_amplitude, num_imfs, num_ensembles);
    return decomposition;

}
