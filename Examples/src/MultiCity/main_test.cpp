/*
 * main_test.cpp
 *
 *  Created on: Apr 11, 2020
 *      Author: forma
 */

#include <iostream>
#include <fstream>

#include "EpidemicModelSIR.hpp"
#include "MultiCityEpidemic.hpp"
#include "MultiCityPopulation.hpp"
#include "PopulationModel.hpp"
#include "MulticityModel.hpp"
#include "MultiCityUtilities.hpp"
int main()
{
  using namespace apsc::multicity;
  constexpr int N=2;
  // Set intitial data for variables
  MultiCityEpidemicVariables<N> mcity;
  MultiCityEpidemicVariablesSISProxy<N> V{mcity};
  V.S().fill(0.0);
  V.I().fill(0.0);
  V.S()(1,1)=24900;
  V.S()(0,0)=25000;
  V.I()(1,1)=100; // 100 infected in city 1
  std::cout<<mcity<<std::endl;

  // Get epidemic data
  MultiCityDataFixed<2> data;
  data.initializeFromArticle();
  data.k_[0]=2.5;
  data.g_[0]=0.05;
  data.g_[1]=0.05;
  // change for SIR
  data.immuneFraction_=1.0;
  std::cout<<data;

  // Compute R0
  auto Ninitial=initialize2Cities{}.initialize();
  auto R0=computeR0(data,Ninitial,0.);
  std::cout<<"RO="<<R0<<std::endl;
  // Advancer object
  MultiCityModelAdvance<N,
  PopulationModel<N,MultiCityDataFixed>,
  EpidemicModelSIR<N,MultiCityDataFixed> > mcAdvance{data};

  // Fill advancer with necessary data
  mcAdvance.initialData.E_Initial=mcity;
  mcAdvance.initialData.N_Initial=initialize2Cities{}.initialize();
  mcAdvance.initialData.tInitial=0; // initial time
  mcAdvance.initialData.tFinal=1000;// End time
  mcAdvance.initialData.numSteps=100; // haoe many time I want output
  mcAdvance.initialData.maxSteps=1000;

  // advance
  auto res=mcAdvance.advance();
  double tfinal;
  std::tie(tfinal,Ninitial)=res.population.back();
  R0=computeR0(data,Ninitial,tfinal);
  std::cout<<"RO="<<R0<<std::endl;
  // dump results
  std::ofstream file("result.txt");
  file<<res;
  file.close();

}


