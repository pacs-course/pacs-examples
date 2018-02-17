#include <iostream>
#include "umfpack_example.hpp"
int main()
{
  int n=5;
  Index Ap = {0, 2, 5, 9, 10, 12};
  Index Ai = { 0, 1, 0, 2, 4, 1, 2, 3, 4, 2, 1, 4};
  Vector Ax = {2., 3., 3., -1., 4., 4., -3., 1., 2., 2., 6., 1.};
  Vector b  = {8., 45., -3., 3., 19.};

  auto x = solve_with_umfpack(n,Ap,Ai,Ax,b);

  std::cout<<"x = ";
  for (auto y:x)std::cout<<y<<" ";
  std::cout<<std::endl;
}
