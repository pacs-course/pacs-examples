/*
 * main_test.cpp
 *
 *  Created on: Apr 11, 2020
 *      Author: forma
 */

#include <iostream>

#include "MultiCityEpidemic.hpp"
int main()
{
  using namespace apsc::multicity;
  constexpr int N=2;
  MultiCityEpidemicVariables<2> mcity;
  MultiCityEpidemicVariables<2>::BlockType S0;
  MultiCityEpidemicVariables<2>::BlockType I0;
  for (int i=0; i<N; ++i)
    for (int j=0; j<N; ++j)
    {
      S0(i,j)=i*j;
      I0(i,j)=0.2;
    }
  mcity.S()=S0;
  mcity.I()=I0;
  std::cout<<"S="<<mcity.S()<<std::endl;
  std::cout<<"I="<<mcity.I()<<std::endl;

  MultiCityDataFixed<2> data;
  data.initializeFromArticle();
  std::cout<<data;
}


