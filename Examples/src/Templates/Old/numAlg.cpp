// comparision in efficiency for passing function object, 
// function pointer, and truely inline code

#include <iostream>
#include <vector>
#include <numeric>
#include <iostream>
#include <ctime>

template<class T> struct SumFO {            // passing function object 
  T operator()(const T& a, const T& b) const { return a + b; }
};

double SumFcn(double a, double b) { return a + b; }  

//template<class T> T SumFcn(T a, T b) { return a + b; }  

int main() {
  using namespace std;

  int n = 10000;
  int m = 80000;
  double* ai = new double [n]; 
  for (int i = 0; i < n; i++) ai[i] = i - n/2;
  double d =  0;

  time_t tm0 = time(0);                   // time_t is defined in <time.h>
  d=0;
  for (int i = 0; i < m; i++)// do it m times
    for (int j=0;j<n;++j)d += ai[j];      // standard loop
  cout << " d = " << d << "\n";

  time_t tm1 = time(0);                   // measur the current time
  cout << "time in default sum = " << tm1 -tm0 << " seconds" << "\n";

  d=0;
  for (int i = 0; i < m; i++)             // do it 100 times
    d += accumulate(ai, ai + n, 0.0);      // summation by default
  cout << " d = " << d << "\n";

  time_t tm2 = time(0);                   // measur the current time
  cout << "time in standard accumulate = " << tm2 -tm1 << " seconds" << "\n";

  d=0;
  for (int i = 0; i < m; i++)             // do it 100 times
    d += accumulate(ai, ai + n, 0.0, SumFcn); // passing a function
  cout << " d = " << d << "\n";

  time_t tm3 = time(0);                   // measur the current time
  cout << "time in passing function = " << tm3 -tm2 << " seconds" << "\n";

  d=0;
  for (int i = 0; i < m; i++)             // do it 100 times
    d += accumulate(ai, ai + n, 0.0, SumFO<double>()); // passing a fcn object
  cout << " d = " << d << "\n";
  time_t tm4 = time(0);                   // measur the current time
  cout << "time in passing fcn object = " << tm4 -tm3 << " seconds" << "\n";

  
}
