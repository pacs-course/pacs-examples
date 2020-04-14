/*
 * main_interp.cpp
 *
 *  Created on: Apr 14, 2020
 *      Author: forma
 */

#include  "interp1D.hpp"
#include <iostream>
#include <functional>


int main()
{
  std::vector< std::array<double,2> > v = {
      {0.1,2.0},
      {0.3,4.0},
      {0.5,6.0},
      {0.55,7.0},
      {0.65,-2.0},
      {0.7,-3.0},
      {0.8,4.0}
  };
  std::vector<double> keys =
      {0, 0.1, 0.35, 0.7,0.75, 0.8,0.9};

  for (auto key : keys){
      double xin=apsc::interp1D(v, key);
      std::cout<< "Value at "<<key<<" equal to "<<xin<<std::endl;
  }
// To check if it works also with other comparison opts
  std::vector< std::array<double,2> > vr{v.rbegin(),v.rend()}; //reverse vecto
  // print it out:
  for (auto i: vr )std::cout<<i[0]<<" "<<i[1]<<std::endl;
 // now is from largest to smaller less<T> is inconsistent: I have to use std::greater

  for (auto key : keys){
      double xin=apsc::interp1D(vr, key,std::greater<double>());
      std::cout<< "Value at "<<key<<" equal to "<<xin<<std::endl;
  }



}

