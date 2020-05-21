/*
 * testRotatingVector.cpp
 *
 *  Created on: Nov 18, 2018
 *      Author: forma
 */

#include <iostream>
#include <array>
#include "../RotatingVector.hpp"

template<class T>
void printRV(const T& rv)
{
  std::cout<<" Size ="<<rv.size()<<std::endl;
  for (unsigned int i=0;i<rv.size();++i)
    {
    std::cout<<"element "<<i<<std::endl;
      const auto & elem=rv[i];
      for (unsigned int j=0;j<elem.size();++j)
        {
          std::cout<<elem[j]<<" ";
        }
      std::cout<<std::endl;
    }
  std::cout<<"---"<<std::endl;
}
int main()
{
  using namespace std;
  using namespace apsc;
  std::cout<<"I create a rotatingvector of size 3 of arrays of size 3"<<std::endl;
  RotatingVector<array<double,3>,3> rv;
  std::cout<<" adding (1,1,1) "<<std::endl;
  rv.emplace_back(array<double,3>{1.,1.,1.});
  printRV(rv);
  std::cout<<" adding (2,2,2) "<<std::endl;
  rv.emplace_back(array<double,3>{2.,2.,2.});
  printRV(rv);
  std::cout<<" adding (3,3,3) "<<std::endl;
  rv.emplace_back(array<double,3>{3.,3.,3.});
  printRV(rv);
  std::cout<<" adding (4,4,4) "<<std::endl;
  rv.emplace_back(array<double,3>{4.,4.,4.});
  printRV(rv);
  std::cout<<" adding (5,5,5) "<<std::endl;
  rv.emplace_back(array<double,3>{5.,5.,5.});
  printRV(rv);
  std::cout<<" adding (6,6,6) "<<std::endl;
  rv.emplace_back(array<double,3>{6.,6.,6.});
  printRV(rv); 

  
  //  to test copy constuction
  //  RotatingVector<array<double,3>,3> copyRV=rv;
  //  printRV(copyRV);

  std::cout<<" Now RotatingVectorXd"<<std::endl;
  std::cout<<"I create a rotatingvector of maximal size 4 of arrays of size 3"<<std::endl;
  RotatingVectorXd<array<double,3>> rvx;
  rvx.setMaxSize(4);
  std::cout<<" adding (1,1,1) "<<std::endl;
  rvx.emplace_back(array<double,3>{1.,1.,1.});
  printRV(rvx);
  rvx.emplace_back(array<double,3>{2.,2.,2.});
  std::cout<<" adding (2,2,2) "<<std::endl;
  printRV(rvx);
  std::cout<<" adding (3,3,3) "<<std::endl;
  rvx.emplace_back(array<double,3>{3.,3.,3.});
  printRV(rvx);
  std::cout<<" adding (4,4,4) "<<std::endl;
  rvx.emplace_back(array<double,3>{4.,4.,4.});
  printRV(rvx);
  std::cout<<" adding (5,5,5) "<<std::endl;
  rvx.emplace_back(array<double,3>{5.,5.,5.});
  printRV(rvx);
  std::cout<<" adding (6,6,6) "<<std::endl;
  rvx.emplace_back(array<double,3>{6.,6.,6.});
  printRV(rvx);
  std::cout<<"The last element   "<<rvx.back()[0]<<","<<rvx.back()[1]<<","<<rvx.back()[2]<<std::endl;
  std::cout<<"The last-1 element "<<rvx.back<1>()[0]<<","<<rvx.back<1>()[1]<<","<<rvx.back<1>()[2]<<std::endl;
  std::cout<<"I can shrink to max 3"<<std::endl;
  rvx.setMaxSize(3);
  printRV(rvx);

  // RotatingVectorXd<array<double,3>> copyRVx=rvx;
  // printRV(copyRVx);


  
}



