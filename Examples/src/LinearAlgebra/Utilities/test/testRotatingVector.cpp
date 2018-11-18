/*
 * testRotatingVector.cpp
 *
 *  Created on: Nov 18, 2018
 *      Author: forma
 */

#include <iostream>
#include <array>
#include "../RotatingVector.hpp"

void printRV(LinearAlgebra::RotatingVector<std::array<double,3>,3> const & rv)
{
  for (unsigned int i=0;i<rv.size();++i)
    {
      std::cout<<rv[i][0]<<" "<<rv[i][1]<<" "<<rv[i][2]<<std::endl;
    }
  std::cout<<std::endl;
}
int main()
{
  using namespace std;
  using namespace LinearAlgebra;
  RotatingVector<array<double,3>,3> rv;
  rv.emplace_back(array<double,3>{1.,1.,1.});
  printRV(rv);
  rv.emplace_back(array<double,3>{2.,2.,2.});
  printRV(rv);
  rv.emplace_back(array<double,3>{3.,3.,3.});
  printRV(rv);
  rv.emplace_back(array<double,3>{4.,4.,4.});
  printRV(rv);
  rv.emplace_back(array<double,3>{5.,5.,5.});
  printRV(rv);
  rv.emplace_back(array<double,3>{6.,6.,6.});
  printRV(rv);

}



