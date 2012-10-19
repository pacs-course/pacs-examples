#include <iostream>                     // for output
#include <ctime>                        // for measuring time

int main() {

  using namespace std;

  int n = 100000000;
  time_t tm0 = time(0);                   // wall time at this point 
  clock_t ck0 = clock();                  // clock ticks at this point 

  double d, dpi = 3.1415926535897932385;
  float  f, fpi = 3.1415926535897932385;
  for (int i = 0; i< n; i++) d = (double(i) + dpi)*dpi;

  time_t tm1 = time(0);                   // wall time at this point 
  clock_t ck1 = clock();                  // clock ticks at this point 
  cout << "wall time = " << difftime(tm1, tm0) << " seconds.\n";
  cout << "CPU time = " << double(ck1 -ck0)/CLOCKS_PER_SEC << " seconds.\n";

  for (int i = 0; i< n; i++) f = (float(i) + fpi)*fpi; 

  time_t tm2 = time(0);                   // wall time at this point 
  clock_t ck2 = clock();                  // clock ticks at this point 
  cout << "wall time = " << difftime(tm2, tm1) << " seconds.\n";
  cout << "CPU time = " << double(ck2 -ck1)/CLOCKS_PER_SEC << " seconds.\n";

  cout << "The current time is: " << ctime(&tm2) << "\n";
}
