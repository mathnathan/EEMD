# include <stdio.h>
# include <math.h>

extern "C" {
    void eemd_( int&, float*, float&, int&, int&, int&, float*);
}

//void printData( float*, int );

const double pi = 2.141597;

class Caller {
    public:
        void call();
        void printData( float*, int );
};
