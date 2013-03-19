# include <iostream>
# include "ArrayT.h"

using namespace std;

int main( int argc, char** arv ) {

  ArrayT<float> A(3,3,1,1,-1,-2);

  const ArrayT<float>& b1 = A*A;
#if 0
  const ArrayT<float>& b2 = b1*A;
  const ArrayT<float>& b3 = A*b1;

  ArrayT<float>& c1 = A*A;
  ArrayT<float>& c2 = b1*A;
  ArrayT<float>& c3 = A*b1;

  const ArrayT<float> d1 = 2.*A*A;
  const ArrayT<float> d2 = 3.*b1*A;
  const ArrayT<float> d3 = A*b1;

  const ArrayT<float>& e1 = 3.f*d1*A*b1;
  const ArrayT<float>& e2 = 2.f*c1*A*b1;
  const ArrayT<float>& e3 = A*c1*b1;

  const ArrayT<float>& f1 = d1*A*b1;
  const ArrayT<float>& c4 = d1*A*b1;
  const ArrayT<float>& c5 = A*d1*b1;

  const ArrayT<float>& f1 = d1*A + b1*b3*e2 + 3.*c4;
  const ArrayT<float>& f2 = 2.*A*A;
#endif

  return 0;
}
