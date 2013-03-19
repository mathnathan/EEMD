# include "cppcall.h"

void Caller::call() {

    int lxy = 40; //
    float step = (4*pi)/lxy;
    float* indata = new float[lxy]; //
    for( int i=0; i<lxy; i++ ) {
        indata[i] = cos(i*step);
    }
    float An = 0.2; //
    int Nesb = 100; //
    int Nm = 10; //
    int idum = 3471; //
    float* rslt = new float[lxy*(Nm+2)]; //

    printData( indata, lxy );

    eemd_( lxy, indata, An, Nesb, Nm, idum, rslt );

}

void Caller::printData( float* input, int len ) {

    printf( "len = %d\n", len );
    for( int i=0; i<len; i++ ) {
        printf( "elem[%d] = %f\n", i, input[i] );
    }

}
