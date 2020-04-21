/*
 * main_test.cpp
 *
 *  Created on: Apr 11, 2020
 *      Author: forma
 */

#include <iostream>
#include <fstream>

#include "MultiCityEpidemic.hpp"
#include "MultiCityPopulation.hpp"
#include "PopulationModel.hpp"
#include "EpidemicModel.hpp"
#include "MulticityModel.hpp"
int main()
{
  using namespace apsc::multicity;
  constexpr int N=2;
  // Set intitial data for variables
  MultiCityEpidemicVariables<N> mcity;
  MultiCityEpidemicVariablesProxy<N> V{mcity};
  V.S().fill(0.0);
  V.I().fill(0.0);
  V.S()(1,1)=2500;
  V.S()(0,0)=2400;
  V.I()(0,0)=100; // 100 infected in city 0
  std::cout<<mcity<<std::endl;

  // Get epidemic data
  MultiCityDataFixed<2> data;
  data.initializeFromArticle();
  std::cout<<data;

  // Advancer object
  MultiCityModelAdvance<N,
  PopulationModel<N,MultiCityDataFixed>,
  EpidemicModel<N,MultiCityDataFixed> > mcAdvance{data};

  // Fill advancer with necessary data
  mcAdvance.initialData.E_Initial=mcity;
  mcAdvance.initialData.N_Initial=initialize2Cities{}.initialize();
  mcAdvance.initialData.tInitial=0; // initial time
  mcAdvance.initialData.tFinal=1000;// End time
  mcAdvance.initialData.numSteps=100; // haoe many time I want output

  // advance
  auto res=mcAdvance.advance();

  // dump results
  std::ofstream file("result.txt");
  file<<res;
  file.close();

}


