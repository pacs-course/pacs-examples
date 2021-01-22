/*
 * test_joinVectors.cpp
 *
 *  Created on: Jan 21, 2021
 *      Author: forma
 */
#include <iostream>
#include <complex>

#include "JoinVectors.hpp"
int main()
{
  std::vector<int> vi;
  std::vector<double> vd;
  std::vector<std::complex<double>> vc;
  auto joined = apsc::join_vectors(vi,vd,vc);
  int N=10;
  vi.resize(N,9);
  vd.resize(N,3.4);
  vc.resize(N,{4.0,3.0});
  vi.back()=40;
  vd.back()=-10.;

  // Loop over the joined vectors
  std::cout<<"*********** Joined vector content:\n";
  for (std::size_t i=0;i<joined.size();++i)
    {
      auto [k,x,c] = joined[i];
      std::cout<<k<<" "<<x<<" "<<c<< std::endl;
    }
  std::cout<<"*********** you can also loop via a range based for:\n";
  for (auto [k,x,c]:joined)
    std::cout<<k<<" "<<x<<" "<<c<< std::endl;
  std::cout<<"*********** or a loop with iterators:\n";
  for (auto  i=joined.begin(); i!=joined.end();++i)
    {
      auto const [k,x,c] = *i;
      std::cout<<k<<" "<<x<<" "<<c<< std::endl;
    }
  // Use of the access through pointer operator (here the "pointer" is the iterator).
    auto it=joined.begin();
    std::cout<<"\n The size of the stored joned vectors is: "<<it->size()<<std::endl;


}



