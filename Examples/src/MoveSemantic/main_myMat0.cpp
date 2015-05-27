#include <iostream>
#include <vector>
#include <fstream>
#include "MyMat0.hpp"

//! A factory that builds a Hilbert matrix
LinearAlgebra::MyMat0<LinearAlgebra::ROWMAJOR> Hilbert(unsigned int n);

int main()
{
  using namespace LinearAlgebra;
  // I create two matrices
  MyMat0<COLUMNMAJOR> a;
  //MyMat0 a;
  a.resize(3,4);
  MyMat0<> b(5,5);

  b.fillRandom();

  std::cout<< "Testing copy constructors and move semantic"<<std::endl;
  MyMat0<ROWMAJOR> t(a);
  std::cout<<" This matrix should be equal to a"<<std::endl;
  t.showMe();
  MyMat0<ROWMAJOR> tc(std::move(t));
  std::cout<<" This matrix should be also equal to a"<<std::endl;
  tc.showMe();
  std::cout<<" This matrix is now empty"<< std::endl;
  t.showMe();
  t=std::move(tc);
  std::cout<<" Now it is back"<<std::endl;
  t.showMe();
  std::cout<<" And this is empty"<<std::endl;
  tc.showMe();

  /* SHOW THE ADVANTAGES OF MOVE SEMANTIC */
  constexpr unsigned int N=10000;
  MyMat0<ROWMAJOR> matH=Hilbert(N);
  
  std::cout<<" Created a matrix of "<<N*N*8/1000000<<" Mbytes"<<std::endl;

  // Swap with another matrix.

  MyMat0<ROWMAJOR> tmp = std::move( matH);
  matH=std::move(tc);
  tc=std::move(tmp);
  
}

LinearAlgebra::MyMat0<LinearAlgebra::ROWMAJOR> Hilbert(unsigned int n)
{
  LinearAlgebra::MyMat0<LinearAlgebra::ROWMAJOR> tmp(n,n);
  for (unsigned int i = 0; i<n;++i)
    for (unsigned int j = 0; j<n;++j) tmp(i,j)=1.0/(i+j+1);
  return tmp;
}

