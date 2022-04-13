#include "MyMat0.hpp"
#include <fstream>
#include <iostream>
#include <vector>

//! A factory that builds a Hilbert matrix
LinearAlgebra::MyMat0<LinearAlgebra::ROWMAJOR> Hilbert(unsigned int n);

int
main()
{
  using namespace LinearAlgebra;

  /* SHOW THE ADVANTAGES OF MOVE SEMANTIC */
  constexpr unsigned int N = 6000;
  MyMat0<ROWMAJOR>       matH = Hilbert(N);

  std::cout << " Created a matrix of " << N * N * 8 / 1000000 << " Mbytes"
            << std::endl;
  std::cout << " Created a second matrix of " << N * N * 8 / 1000000
            << " Mbytes" << std::endl;

  MyMat0<ROWMAJOR> tc = Hilbert(N);

  // Swap with another matrix.
  // move activate the move only if MyMat0 has move semantic activated
  // If I have compiled with -DNOMOVE MyMat0 has not move semantic, so std::move
  // does not move anything I do a normal copy!
  MyMat0<ROWMAJOR> tmp = std::move(matH);
  matH = std::move(tc);
  tc = std::move(tmp);
}

LinearAlgebra::MyMat0<LinearAlgebra::ROWMAJOR>
Hilbert(unsigned int n)
{
  LinearAlgebra::MyMat0<LinearAlgebra::ROWMAJOR> tmp(n, n);
  for(unsigned int i = 0; i < n; ++i)
    for(unsigned int j = 0; j < n; ++j)
      tmp(i, j) = 1.0 / (i + j + 1);
  return tmp;
}
