#include <iostream>                     // for output
#include <ctime>                        // for measuring time
#include <cstdlib>                      // for pseudo random number 
#include <algorithm>                    // for sort()

int main() {

  int n = 100000;
  double* dp = new double [n];

  std::srand(time(0));                  // seed the random number generator
  for (int i = 0; i< n; i++) dp[i]  = std::rand()%1000;

  std::sort(dp, dp+n);                  // sort the array in increasing order
}
