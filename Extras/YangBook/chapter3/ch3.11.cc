#include <cerrno>
#include <cfloat>
#include <iostream>
#include <climits>
#include <cmath>

int main() {

using namespace std;

errno = 0;                           // clear old error state. Include <errno.h>
double x = - 50;                     // or x is computed at run time 
double i = sqrt(x);
if (errno == EDOM) cerr << "sqrt() not defined for negative numbers\n";
double p = pow(DBL_MAX, 2);
if (errno == ERANGE) cerr << "result of pow(double,int) overflows\n";

errno = 0;                           // clear old error state. Include <errno.h>
int jj = INT_MAX + 5;
if (errno == ERANGE) cerr << "result of jj overflows\n";

}
