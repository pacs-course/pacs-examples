/* A simple function to illustrate some basic features of pybind11. It adds all
   integers between two given integers and outputs the sum to the screen. */
#include <algorithm> // for std::swap
#include "sumInt.hpp"
int sumInt(int n, int m)
{
  using std::swap; // brings swap in the current scope
  if(n > m)
    swap(n, m); // if n is bigger than m, swap them
  int sum{0};   // sum is initialized to 0
  // a loop, i changes from n to m with increment 1 each time
  for(auto i = n; i <= m; ++i)
    sum += i;
  return sum;
}
