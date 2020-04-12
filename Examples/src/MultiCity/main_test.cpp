/*
 * main_test.cpp
 *
 *  Created on: Apr 11, 2020
 *      Author: forma
 */

#include <iostream>

#include "MultiCityEpidemic.hpp"
#include "MultiCityPopulation.hpp"
#include "PopulationModel.hpp"
int main()
{
  using namespace apsc::multicity;
  constexpr int N=2;
  MultiCityEpidemicVariables<N> mcity;
  MultiCityEpidemicVariables<N>::BlockType S0;
  MultiCityEpidemicVariables<N>::BlockType I0;
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

  MultiCityPopulationVariables<N> pop;
  initialize2Cities::initialize(pop);
  std::cout<<"Population Data\n";
  std::cout<<"N\n"<<pop.N<<std::endl;
  std::cout<<"Np\n"<<pop.Np<<std::endl;
  std::cout<<"Nr\n"<<pop.Nr;
  std::cout<<std::endl;
  PopulationModel<N,MultiCityDataFixed> modelP{data};
  auto force = modelP(2.0, pop.N);
  std::cout<<"forcing tem\n"<<force;
  std::cout<<std::endl;



}


