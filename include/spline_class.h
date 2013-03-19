/* spline.c
   Cubic interpolating spline. */

/* Gordon Erlebacher
 * Transform to a class to simplify usage. Interface to spline.c 
 */

#ifndef _SPLINE_H_
#define _SPLINE_H_

#include <vector>

class Spline
{
public:
std::vector<double> x, y, b, c, d;
int nbpts;
int last;
int iflag; // error flag

// need constructor
Spline();
Spline(std::vector<double> x, std::vector<double> y);
void push_back(double x, double y);
double operator()(double x); // evaluate the spline

//private:
int spline (int end1, int end2,
            double slope1, double slope2);

double eval (double u);
};
#endif
