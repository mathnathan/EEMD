#include "meemd.h"
#include <stdio.h>
#include <stdexcept>
#include <netcdf.h>
#include "util.h"

Util util;

const bool VERBOSE = 0;

//----------------------------------------------------------------------
MEEMD::MEEMD( const std::string& filename, int type ) {
if(VERBOSE) printf("\nMEEMD::MEEMD( const std::string& filename=%s )\n", filename.c_str() );

    if( load( filename, type ) ) {
        printf("signal cols = %d\n", signal.n_cols);
        eemd = new EEMD( signal.n_cols );
    }
    else {
        printf( "\nERROR: Could not load file, \"%s\"\n", filename.c_str() );
        exit( EXIT_FAILURE );
    }
}
//----------------------------------------------------------------------
MEEMD::MEEMD() {
if(VERBOSE) printf("\nMEEMD::MEEMD()\n");

    eemd = new EEMD();
}
//----------------------------------------------------------------------
MEEMD::~MEEMD() {
if(VERBOSE) printf("\nMEEMD::~MEEMD()\n");

    delete eemd;
}
//----------------------------------------------------------------------
bool MEEMD::load( const std::string& filename, int type  ) {
if(VERBOSE) printf("\nbool MEEMD::load( const std::string& filename=%s, int type=%d )\n", filename.c_str(), type );

    if( type == MEEMD_NETCDF ) {
        int status, ncid, latid, lonid, recid, sschlorid;
        size_t latLen, lonLen, recs;
        char recname[NC_MAX_NAME+1];

        printf( "Filename = %s\n", filename.c_str() );
        if( (status = nc_open( filename.c_str(), NC_NOWRITE, &ncid)) ) {
            printf("Error: %s\n", nc_strerror(status)); exit(2); }

#if 0
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

        static size_t start[] = {0,0,0};
        static size_t count[] = {recs, latLen, lonLen};
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
#endif

        std::vector<int> signal_base( sschlor, sschlor+array_size );
        signal = arma::conv_to<MAT>::from(signal_base);

        float max, min;
        max = signal.max();
        min = signal.min();
        printf( "max = %f\n", max );
        printf( "min = %f\n", min );
        printf( "ylen = %ld\n", ylen );
        printf( "xlen = %ld\n", xlen );
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
    else if( type == MEEMD_IMAGE ) {
        signal = util.load_img( filename );
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
void MEEMD::show() {
if(VERBOSE) printf("\nvoid MEEMD::show()\n");

    int sz = final_imfs.size();
    char name[50];
    for( int i=0; i < sz; i++ ) {
        sprintf( name, "IMF %d", i );
        util.show_mat( final_imfs[i], name, false );
    }
    sprintf( name, "Original" );
    util.show_img( signal, name );
}
//----------------------------------------------------------------------
void MEEMD::showimf( int which_imf ) {
if(VERBOSE) printf("\nvoid MEEMD::showimf( int which_imf=%d )\n", which_imf);
    char name[50];
    sprintf( name, "IMF %d", which_imf );
    util.show_mat( final_imfs[which_imf], name );
}
//----------------------------------------------------------------------
bool MEEMD::save( const MAT& input, const std::string& filename ) {
if(VERBOSE) printf("\nbool MEEMD::save( const std::string& filename=%s, const MAT& input )\n", filename.c_str() );

    try { util.save_mat( input, filename.c_str() ); }
    catch ( std::runtime_error& ex) {
        fprintf(stderr, "Error writing image: %s\n\n\tException: %s",
                filename.c_str(), ex.what());
        return false; // Let user handle it by returning a 'false' for now
    }

    return true;
}
//----------------------------------------------------------------------
bool MEEMD::save( const std::string& destination ) {
if(VERBOSE) printf("\nbool MEEMD::save()\n");

    char command[255];
    char* cmd;
    strcpy( cmd, "mkdir -p ");
    strcpy( command, cmd );
    strcat( command, destination.c_str() );
    system( command );
    int sz = final_imfs.size();
    char path[100];
    char name[16];
    for( int i=0; i < sz; i++ ) {
        strcpy( path, destination.c_str() );
        sprintf( name, "imf%d.jpg", i );
        strcat( path, name );
        try{ util.save_mat( final_imfs[i], path ); }
        catch ( std::runtime_error& ex) {
            fprintf(stderr, "Error writing image, %s, to %s\n\n\tException: %s",
                        name, destination.c_str(), ex.what());
            return false; // Let user handle it by returning a 'false' for now
        }
    }
    sprintf( name, "original.jpg" );
    strcpy( path, destination.c_str() );
    strcat( path, name );
    try{ util.save_mat( signal, path ); }
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
    ROW r = inMAT.row(0);
    MAT tmp_imfs = eemd->eemdf90(r, noise_amplitude, nb_imfs, nb_noise_iters);

    for( int j=0; j<nb_imfs; j++ ) {
        g.push_back( MAT( tmp_imfs.row(j) ) );
    }

    // Now loop through the rest
    for( int m=1; m<rows; m++ ) {
        printf("Operating on row %d out of %d\n", m, rows);
        ROW r = inMAT.row(m);
        tmp_imfs = eemd->eemdf90(r, noise_amplitude, nb_imfs, nb_noise_iters);
        if( nb_imfs != tmp_imfs.n_rows ) {
            printf("\nROWS DIFFERENT # of IMFS!\n");
            printf("nb_imfs = %d, tmp_imfs.n_rows = %d\n", nb_imfs, tmp_imfs.n_rows);
            //exit(2);
        }
        for( int j=0; j<nb_imfs; j++ ) {
            g[j].insert_rows( m, tmp_imfs.row(j) );
        }
    }

    char name[50];
    char fname[50];
    static int rcntr = 0;
    rcntr = rcntr + 1;

    return g;
}
//----------------------------------------------------------------------
std::vector<MAT> MEEMD::decomposeCols( const MAT& inMAT, float noise_amplitude, int nb_imfs, int nb_noise_iters ) {
if(VERBOSE) printf("\nstd::vector<MAT> MEEMD::decomposeCols( const MAT& inMAT, float noise_amplitude=%f, int nb_imfs=%d, int nb_noise_iters=%d )\n", noise_amplitude, nb_imfs, nb_noise_iters );

    std::vector<MAT> g; // Store the col decompositions of inMAT
    int cols = inMAT.n_cols;

    // First iteration to set things up
    VEC c = inMAT.col(0);
    MAT tmp_imfs = eemd->eemdf90(c, noise_amplitude, nb_imfs, nb_noise_iters);

    for( int j=0; j<nb_imfs; j++ ) {
        g.push_back( MAT( tmp_imfs.col(j) ) );
    }

    // Now loop through the rest
    for( int n=1; n<cols; n++ ) {
        printf("Operating on col %d out of %d\n", n, cols);
        VEC c = inMAT.col(n);
        tmp_imfs = eemd->eemdf90(c, noise_amplitude, nb_imfs, nb_noise_iters);
        if( nb_imfs != tmp_imfs.n_cols ) {
            printf("\nCOLS DIFFERENT # of IMFS!\n");
            printf("nb_imfs = %d, tmp_imfs.n_cols = %d\n", nb_imfs, tmp_imfs.n_cols);
        }
        for( int j=0; j<nb_imfs; j++ ) {
            g[j].insert_cols( n, tmp_imfs.col(j) );
        }
    }

    char name[50];
    char fname[50];
    static int ccntr = 0;
    ccntr = ccntr + 1;

    return g;

}
//----------------------------------------------------------------------
void MEEMD::decompose( float noise_amplitude, int nb_imfs, int nb_noise_iters ) {
if(VERBOSE) printf("\nvoid MEEMD::decompose( float noise_amplitude=%f, int nb_imfs=%d, int nb_noise_iters=%d )\n", noise_amplitude, nb_imfs, nb_noise_iters );

    std::vector<MAT> g; // Store the row decompositions of signal
    std::vector<MAT> g_col; // Store the col decomps of g

    g = decomposeRows( signal, noise_amplitude, nb_imfs, nb_noise_iters );

    // Then decompose the cols of each gj IMF
    for( int j=0; j<g.size(); j++ ) {
        printf("Operating on IMF %d out of %ld\n", j, g.size() );

        g_col = decomposeCols( g[j], noise_amplitude, nb_imfs, nb_noise_iters );
        // The below line means h[j][k] = jth row decomp, kth col decomp
        h.push_back( g_col );
    }
}
//----------------------------------------------------------------------
void MEEMD::combine() { 
if(VERBOSE) printf("\nvoid MEEMD::combine()\n");

    int r = h.size();
    int c = h[0].size();
    int lim = c > r ? r : c;
    MAT tmp1(h[0][0]); 
    MAT tmp2(h[0][0]);
        
    for( int i=0; i<lim; i++ ) {
        tmp1.zeros();
        tmp2.zeros();
        for( int k=i; k<r; k++ ) {
            tmp1 += h[i][k]; 
        }   
        for( int j=i+1; j<c; j++ ) {
            tmp2 += h[j][i];
        }

        tmp1 += tmp2;
        final_imfs.push_back( tmp1 );
    }
}
//----------------------------------------------------------------------



