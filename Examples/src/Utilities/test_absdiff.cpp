/*
 * test_absdiff.cpp
 *
 *  Created on: Sep 28, 2022
 *      Author: forma
 */
#include <iostream>
#include <cmath>
#include "absdiff.hpp"
int main()
{
  std::cout<<"|5u-6u| with absdiff  "<<apsc::absdiff(5u, 6u)<<"\n";
//  std::cout<<"|5u-6u| with std::abs "<<std::abs(5u-6u)<<"\n";// does not compile!
  std::cout<<"|5u-6u| with std::fabs "<<std::fabs(5u-6u)<<"\n";
  std::cout<<"|5.7-6.8| with asbdiff "<<apsc::absdiff(5.7,6.8)<<"\n";

}



