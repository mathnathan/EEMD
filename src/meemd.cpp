#include "meemd.h"
#include <stdio.h>
#include <stdexcept>
#include <netcdf.h>
#include "tools.h"
#include "eemd.h"
#include <sstream>
#include <ctime>

const bool VERBOSE = 1;

//----------------------------------------------------------------------
MEEMD::MEEMD( const std::string& filename, int type ) {
if(VERBOSE) printf("\nMEEMD::MEEMD( const std::string& filename=%s )\n", filename.c_str() );

    if( load( filename, type ) ) {
        printf("signal cols = %d\n", signal.n_cols);
        eemd = new EEMD( signal.n_cols );
        tools = new Tools;
    }
    else {
        printf( "\nERROR: Could not load file, \"%s\"\n", filename.c_str() );
        exit( EXIT_FAILURE );
    }
}
//----------------------------------------------------------------------
MEEMD::MEEMD() {

    dtype = -1;
    eemd = new EEMD;
    tools = new Tools;
}
//----------------------------------------------------------------------
MEEMD::~MEEMD() {
if(VERBOSE) printf("\nMEEMD::~MEEMD()\n");

    //delete eemd; // This causes a glibc double free error. Not sure why
    delete tools;
    eemd = NULL;
    tools = NULL;
}
//----------------------------------------------------------------------
bool MEEMD::load( const std::string& filename, int type  ) {
if(VERBOSE) printf("\nbool MEEMD::load( const std::string& filename=%s, dtype=%d )\n", filename.c_str(), type );

    dtype = type;

    if( dtype == MEEMD_NETCDF ) {
        int status, ncid, latid, lonid, recid, sschlorid;
        size_t latLen, lonLen, recs;
        char recname[NC_MAX_NAME+1];

        printf( "Filename = %s\n", filename.c_str() );
        if( (status = nc_open( filename.c_str(), NC_NOWRITE, &ncid)) ) {
            printf("Error: %s\n", nc_strerror(status)); exit(2); }

#if 1
        if( (status = nc_inq_unlimdim(ncid, &recid)) ) {
            printf("Error: %s\n", nc_strerror(status)); exit(2); }
        if( (status = nc_inq_dim(ncid, recid, recname, &recs)) ) {
            printf("Error: %s\n", nc_strerror(status)); exit(2); }

        if( (status = nc_inq_dimid(ncid, "Latitude", &latid)) ) {
            printf("Error: %s\n", nc_strerror(status)); exit(2); }
        if( (status = nc_inq_dimlen(ncid, latid, &latLen)) ) {
            printf("Error: %s\n", nc_strerror(status)); exit(2); }

        if( (status = nc_inq_dimid(ncid, "Longitude", &lonid)) ) {
            printf("Error: %s\n", nc_strerror(status)); exit(2); }
        if( (status = nc_inq_dimlen(ncid, lonid, &lonLen)) ) {
            printf("Error: %s\n", nc_strerror(status)); exit(2); }

        if( (status = nc_inq_varid(ncid, "sschlor", &sschlorid)) ) {
            printf("Error: %s\n", nc_strerror(status)); exit(2); }

        // COMMENTED OUT on 04/16/13
        static size_t start[] = {0,0,0};
        static size_t count[] = {recs, latLen, lonLen};
        
        // ** ADDED for testing on 04/16/13
        // Crop the netcdf file to be square and without discontinuities (NANS)
        //lonLen = 140;
        //latLen = 140;
        //static size_t start[] = {0,80,220};
        //static size_t count[] = {recs, latLen, lonLen};
        // ** ADDED for testing
        
        int array_size = recs*latLen*lonLen;
        float sschlor[ array_size ];

        if( (status = nc_get_vara_float(ncid, sschlorid, start, count, sschlor)) ) {
            printf("Error: %s\n", nc_strerror(status)); exit(2); }

        printf( "recid = %d\n", recid );
        printf( "recname = %s\n", recname  );
        printf( "recs = %lu\n", recs );
        printf( "latid = %d\n", latid );
        printf( "latLen = %lu\n", latLen );
        printf( "lonid = %d\n", lonid );
        printf( "lonLen = %lu\n", lonLen );
#else
        int xid, yid, dataid;
        size_t xlen, ylen;
        if( (status = nc_inq_dimid(ncid, "x", &xid)) ) {
            printf("Error: %s\n", nc_strerror(status)); exit(2); }
        if( (status = nc_inq_dimlen(ncid, xid, &xlen)) ) {
            printf("Error: %s\n", nc_strerror(status)); exit(2); }
        if( (status = nc_inq_dimid(ncid, "y", &yid)) ) {
            printf("Error: %s\n", nc_strerror(status)); exit(2); }
        if( (status = nc_inq_dimlen(ncid, yid, &ylen)) ) {
            printf("Error: %s\n", nc_strerror(status)); exit(2); }
        if( (status = nc_inq_varid(ncid, "data", &dataid)) ) {
            printf("Error: %s\n", nc_strerror(status)); exit(2); }

        static size_t start[] = {0,0};
        static size_t count[] = {xlen, ylen};
        int array_size = xlen*ylen;
        float sschlor[ array_size ];

        if( (status = nc_get_vara_float(ncid, dataid, start, count, sschlor)) ) {
            printf("Error: %s\n", nc_strerror(status)); exit(2); }
        latLen = ylen;
        lonLen = xlen;

        float max, min;
        max = signal.max();
        min = signal.min();
        printf( "max = %f\n", max );
        printf( "min = %f\n", min );
        printf( "ylen = %ld\n", ylen );
        printf( "xlen = %ld\n", xlen );
#endif

        std::vector<float> signal_base( sschlor, sschlor+array_size );
        signal = arma::conv_to<MAT>::from(signal_base);

        signal.reshape(latLen, lonLen);
        
    // Only to test data... --------
#if 0
        printf("0 |");
       for(int y=0; y<ylen; y++ ) {
           if(y!=0) {
           printf("%d |", y);
           }
           for( int x=0; x<xlen; x++ ) {
               printf(" %f ", signal(x,y));
           }
            printf("|\n");
       }
#endif
    // -----------------------------

        *eemd = EEMD( signal.n_cols );

        try { }
        catch ( std::runtime_error& ex ) {
            // Also use 'status' to print a more descriptive error
            fprintf(stderr, "Error loading image: %s\n\n\tException: %s",
                    filename.c_str(), ex.what());
            //exit( 1 );
            return false; // Let user handle it by returning a 'false' for now
        }
    }
    else if( dtype == MEEMD_IMAGE ) {
        // OpenCV routines are all packed in the tools class
        signal = tools->load_img( filename );
        *eemd = EEMD( signal.n_cols );
    }
    else {
        return false;
        //printf( "\nERROR: Input data type, %s, not recognized\n" );
        //exit(1);
    }

    return true;
}
//----------------------------------------------------------------------
void MEEMD::showInput() {
if(VERBOSE) printf("\nvoid MEEMD::showInput()\n");

    if( dtype == -1 ) {
        printf( "No input data has been loaded! Use the load routine.\n" );
        exit(EXIT_FAILURE);
    }
        
    char winName[50];
    sprintf( winName, "Input Signal" );
    // Use the same display method for now. Perhaps change in future
    if( dtype == MEEMD_NETCDF ) {
        tools->show_mat( signal, winName ); }
    else if( dtype == MEEMD_IMAGE ) {
        tools->show_mat( signal, winName ); 
    }

}
//----------------------------------------------------------------------
void MEEMD::showOutput() {
if(VERBOSE) printf("\nvoid MEEMD::showOutput()\n");

    if( dtype == -1 ) {
        printf( "There are no results to show! First load data using the load() routine.\n" );
        exit(EXIT_FAILURE);
    }

    int sz = final_imfs.size();
    char name[50];
    for( int i=0; i < sz; i++ ) {
        sprintf( name, "IMF %d", i );
        tools->show_mat( final_imfs[i], name, false );
    }
    tools->wait();

}
//----------------------------------------------------------------------
void MEEMD::showIMF( int which_imf ) {
if(VERBOSE) printf("\nvoid MEEMD::showIMF( int which_imf=%d )\n", which_imf);

    if( dtype == -1 ) {
        printf( "There are no IMFs to show! First load data using the load() routine.\n" );
        exit(EXIT_FAILURE);
    }

    char name[50];
    sprintf( name, "IMF %d", which_imf );
    tools->show_mat( final_imfs[which_imf], name );
}
//----------------------------------------------------------------------
bool MEEMD::save( const MAT& input, const std::string& filename ) {
if(VERBOSE) printf("\nbool MEEMD::save( const std::string& filename=%s, const MAT& input )\n", filename.c_str() );

    try { tools->save_mat( input, filename.c_str() ); }
    catch ( std::runtime_error& ex) {
        fprintf(stderr, "Error writing image: %s\n\n\tException: %s",
                filename.c_str(), ex.what());
        return false; // Let user handle it by returning a 'false' for now
    }

    return true;
}
//----------------------------------------------------------------------
bool MEEMD::save( const std::string& destination ) {
if(VERBOSE) printf("\nbool MEEMD::save( const std::string& destination=%s)\n", destination.c_str());

    char command[256];
    strcpy( command, "mkdir -p ");
    strcat( command, destination.c_str() );
    system( command );
    int sz = final_imfs.size();
    char path[100];
    char name[16];

    for( int i=0; i < sz; i++ ) {
        strcpy( path, destination.c_str() );
        sprintf( name, "imf%d.jpg", i );
        strcat( path, name );
        try{ tools->save_mat( final_imfs[i], path ); }
        catch ( std::runtime_error& ex) {
            fprintf(stderr, "Error writing image, %s, to %s\n\n\tException: %s",
                        name, destination.c_str(), ex.what());
            return false; // Let user handle it by returning a 'false' for now
        }
    }
    sprintf( name, "input.jpg" );
    strcpy( path, destination.c_str() );
    strcat( path, name );
    try{ tools->save_mat( signal, path ); }
    catch ( std::runtime_error& ex) {
            fprintf(stderr, "Error writing image, %s, to %s\n\n\tException: %s",
                        name, destination.c_str(), ex.what());
            return false; // Let user handle it by returning a 'false' for now
        }
}
//----------------------------------------------------------------------
std::vector<MAT> MEEMD::decomposeRows( const MAT& inMAT, float noise_amplitude, int nb_imfs, int nb_noise_iters ) {
if(VERBOSE) printf("\nstd::vector<MAT> MEEMD::decomposeRows( const MAT& inMAT, float noise_amplitude=%f, int nb_imfs=%d, int nb_noise_iters=%d )\n", noise_amplitude, nb_imfs, nb_noise_iters );

    std::vector<MAT> g; // Store the row decompositions of inMAT
    int rows = inMAT.n_rows;

    // First iteration to set things up
    ROW r = inMAT.row(0); // Store the row decompositions of inMat
    MAT imfs = eemd->eemdf90(r, noise_amplitude, nb_imfs, nb_noise_iters);

    // eemdf90 returns the imfs row wise. Row 0 is imf 0, Row 1 is imf
    // 1 and so on. Now we take each imf which came from the 0th row and
    // place it back as the 0th row of each jth imf decomposition matrix. 
    for( int j=0; j<nb_imfs; j++ ) {
        g.push_back( MAT( imfs.row(j) ) );
    }

    // Now loop through the rest
    for( int n=1; n<rows; n++ ) {
        if( n%32 == 0 ) printf("Operating on row %d out of %d\n", n, rows);
        ROW r = inMAT.row(n);
        imfs = eemd->eemdf90(r, noise_amplitude, nb_imfs, nb_noise_iters);
        if( nb_imfs != imfs.n_rows ) {
            printf("\nROWS DIFFERENT # of IMFS!\n");
            printf("nb_imfs = %d, tmp_imfs.n_rows = %d\n", nb_imfs, imfs.n_rows);
            exit(EXIT_FAILURE);
        }

        // Add each nth column onto its respsective kth imf matrix
        for( int k=0; k<nb_imfs; k++ ) {
            g[k].insert_rows( n, imfs.row(k) );
        }
    }

    return g;

}
//----------------------------------------------------------------------
std::vector<MAT> MEEMD::decomposeCols( const MAT& inMAT, float noise_amplitude, int nb_imfs, int nb_noise_iters ) {
if(VERBOSE) printf("\nstd::vector<MAT> MEEMD::decomposeCols( const MAT& inMAT, float noise_amplitude=%f, int nb_imfs=%d, int nb_noise_iters=%d )\n", noise_amplitude, nb_imfs, nb_noise_iters );

    std::vector<MAT> g; // Store the col decompositions of inMAT
    int cols = inMAT.n_cols;

    // First iteration to set things up
    VEC c = inMAT.col(0); // Store the columns of the input matrix
    MAT imfs = eemd->eemdf90(c, noise_amplitude, nb_imfs, nb_noise_iters);

    // eemdf90 returns the imfs column wise. Column 0 is imf 0, column 1 is imf
    // 1 and so on. Now we take each imf which came from the 0th column and
    // place it back as the 0th column of each jth imf decomposition matrix. 
    for( int j=0; j<nb_imfs; j++ ) {
        // Make a matrix with 1 column and push it onto the g vector
        g.push_back( MAT( imfs.col(j) ) ); 
    }

    //char name[32];
    //sprintf( name, "1 column of g[0]" );
    //tools->show_mat( g[0], name );
    //for( int i=0; i<g.size(); i++ ) {
    //    sprintf( name, "1 column g[%d]", i );
    //    tools->show_mat( g[i], name, false );
    //}
    //sprintf( name, "1 column g[%ld]", g.size() );
    //tools->show_mat( g[g.size()-1], name );

    // Now loop through the rest
    for( int m=1; m<cols; m++ ) {
        if( m%32 == 0 ) printf("Operating on col %d out of %d\n", m, cols);
        VEC c = inMAT.col(m); // Store the columns of the input matrix
        imfs = eemd->eemdf90(c, noise_amplitude, nb_imfs, nb_noise_iters);
        if( nb_imfs != imfs.n_cols ) {
            printf("\nCOLS DIFFERENT # of IMFS!\n");
            printf("nb_imfs = %d, tmp_imfs.n_cols = %d\n", nb_imfs, imfs.n_cols);
            exit(EXIT_FAILURE);
        }

        // Add each mth column onto its respsective jth imf matrix
        for( int j=0; j<nb_imfs; j++ ) {
            g[j].insert_cols( m, imfs.col(j) );
        }
        //sprintf( name, "%d columns of g[0]", m );
        //tools->show_mat( g[0], name );
    }

    return g;

}
//----------------------------------------------------------------------
// This is the function the programmer will use to decompose their signal. It
// currently only supports 2 dimensional signals. The variables are self
// descriptive with the exception of perhaps "direction", which is used to
// indicate which direction to decompose first. 
void MEEMD::decompose( float noise_amplitude, int nb_noise_iters, int nb_imfs, int direction ) {
if(VERBOSE) printf("\nvoid MEEMD::decompose( float noise_amplitude=%f, int nb_imfs=%d, int nb_noise_iters=%d )\n", noise_amplitude, nb_imfs, nb_noise_iters );

    std::vector<MAT> g1; // Store the first direction decompositions of signal
    std::vector<MAT> g2; // Store the second direction decomps of g1
    int nb_col_imfs = nb_imfs; // input value
    int nb_row_imfs = nb_imfs; // input value

    if( nb_imfs == -1 ) { // -1 means there was no value given
        nb_col_imfs = log( signal.n_rows ); // log of signal length=number of columns
        nb_row_imfs = log( signal.n_cols ); // log of signal length=number of rows
    }

    if( direction == -1 ) { // -1 means there was no value given
        direction = nb_row_imfs > nb_col_imfs ? 1 : 0; // Decompose first the direction 
                                                       // with the largest axis
    }

    MAT test(3,4);
    float ctr=1.0;
    for(int i=0; i<3; i++) {
        for(int j=0; j<4; j++) {
            test(i,j) = ctr;
            ctr += 1.0;
        }
    }
    tools->show_mat(test, "test case");
    printf("nb_col_imfs = %d\n", nb_col_imfs);
    printf("nb_row_imfs = %d\n", nb_row_imfs);

    printf("cols=%d\n",signal.n_cols);
    printf("rows=%d\n",signal.n_rows);
    for( int i=0; i<signal.n_rows; i++ ) {
        signal.row(i) = signal.row(100);
    }
    tools->show_mat(signal,"original signal");

    if( direction == 0 ) { // 0 means to decompose the columns first
        printf("direction = %d = columns\n", direction);
        printf("\nDecomposing Columns...\n");
        g1 = decomposeCols( signal, noise_amplitude, nb_col_imfs, nb_noise_iters );

// BEGIN: SHOW COL DECOMP MATS
    char name[32];
    for( int i=0; i<g1.size(); i++ ) {
        sprintf( name, "g1[%d]", i );
        tools->show_mat( g1[i], name, false );
    }
    sprintf( name, "g1[%ld]", g1.size() );
    tools->show_mat( g1[g1.size()-1], name );
// END: SHOW COL DECOMP MATS

        // Then decompose the rows of each g1j IMF
        for( int j=0; j<g1.size(); j++ ) {
            printf("Operating on IMF %d out of %ld\n", j, g1.size() );

            g2 = decomposeRows( g1[j], noise_amplitude, nb_row_imfs, nb_noise_iters );
            // The below line means h[j][k] = jth col decomp, kth row decomp
            h.push_back( g2 );
        }
    } else if( direction == 1 ) { // 1 means to decompose the rows first
        printf("direction = %d = rows\n", direction);
        printf("\nDecomposing Rows...\n");
        g1 = decomposeRows( signal, noise_amplitude, nb_row_imfs, nb_noise_iters );

// BEGIN: SHOW COL DECOMP MATS
    char name[32];
    for( int i=0; i<g1.size(); i++ ) {
        sprintf( name, "g1[%d]", i );
        tools->show_mat( g1[i], name, false );
    }
    sprintf( name, "g1[%ld]", g1.size() );
    tools->show_mat( g1[g1.size()-1], name );
// END: SHOW COL DECOMP MATS

        // Then decompose the columns of each g1j IMF
        for( int j=0; j<g1.size(); j++ ) {
            printf("Operating on IMF %d out of %ld\n", j, g1.size() );

            g2 = decomposeCols( g1[j], noise_amplitude, nb_row_imfs, nb_noise_iters );
            // The below line means h[j][k] = jth row decomp, kth col decomp
            h.push_back( g2 );
        }
    } else {
        printf("direction = %d = UNKNOWN\n", direction);
        printf("\nERROR: direction must be either 0 or 1\n");
        exit( EXIT_FAILURE );
    }
}
//----------------------------------------------------------------------
// This is the function the programmer will use to combine the decompositions
// into the relevant intrinsic mode functions. It will return the IMFS.
// Currently there is no need for it to be an independent function, however in
// the event I would like to change the combination strategy, it would be nice
// to simply pass a parameter to the function to control its behavior.
std::vector<MAT> MEEMD::combine() { 
if(VERBOSE) printf("\nvoid MEEMD::combine()\n");

    int c = h.size(); // Number of columns 
    int r = h[0].size(); // Number of rows

    int lim = c > r ? r : c;
    printf("c=%d\nr=%d\nlim=%d\n",c,r,lim);
    MAT tmp1(h[0][0]); 
    MAT tmp2(h[0][0]);
        
    for( int i=0; i<lim; i++ ) {
        tmp1.zeros();
        tmp2.zeros();
        for( int k=i; k<c; k++ ) {
            tmp1 += h[i][k]; 
        }   
        for( int j=i+1; j<r; j++ ) {
            tmp2 += h[j][i];
        }

        MAT imf = tmp1 + tmp2;
        final_imfs.push_back( imf );
    }

    return final_imfs;
}
//----------------------------------------------------------------------
