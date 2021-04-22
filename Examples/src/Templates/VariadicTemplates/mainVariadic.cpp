/*
 * mainVariadic.cpp
 *
 *  Created on: Mar 30, 2020
 *      Author: forma
 */
#include "folding.hpp"
#include "allContainers.hpp"
#include "perfectForwarding.hpp"
#include <vector>
#include <set>
#include <string>
int main()
{
  using namespace apsc;
  std::cout<<"Testing fold expressions\n";
  double * p=nullptr; // a null puntier converts to false
  bool yes=true;
  int j=0; // An int converts to bool. 0 means false
  std::cout<<std::boolalpha<<" Are all true?"<<allTrue(j,yes,true,p)<<std::endl;;
  double x=10;
  p = &x; // a non null pointer maps to true
  j = 2; // an int different from 0 maps to true
  std::cout<<std::boolalpha<<" Are all true?"<<allTrue(j,yes,true,p)<<std::endl;

  auto res = allSum(x,3,6,8.5,-3);
  std::cout<<" The sum is "<<res<<std::endl;
  std::cout<<" The mean is "<<apsc::allMean(x,3,6,8.5,-3)<<std::endl;

  std::cout<<" The product is "<<apsc::allProd(x,3,6,8.5,-3)<<std::endl;

  std::string a1{"This "};
  std::string a2{"is "};
  std::string a3{"the "};
  std::string a4{"sum "};
  std::string a5{"of 5 strings\n "};
  std::cout<<allSum(a1,a2,a3,a4,a5);

  std::vector<int> vi{1,2,3,4,5,6};// vector has (at least) 2 template parameters
  std::set<double> sd{2.,3.4,-9.0,3.7};// set has (at least) 3 template parameters
  std::array<int,3> ai={-1,-2,-3};
  apsc::printContainerValues(vi);// vector has (at least) 2 template parameters
  apsc::printContainerValues(sd);// set has (at least) 3 template parameters
  // It does not work with std::arrays: the 2nd template parameter is a value not a type.
  // But it works also with unordered_set and list.

  apsc::SuperSmart<apsc::Base> ss1; // empty
  ss1.setValue<Base>(3); // resource is of Base class
  apsc::SuperSmart<apsc::Base> ss2; // empty
  ss2.setValue<Derived>(4,x); //resource is of Derived class

  apsc::SuperSmart<apsc::Base> ss3=ss2; // deep copy!!!
  (*ss1).showMe();
  (*ss3).showMe();
  // I can also override the resource
  ss1.setValue<Derived>(40,-150);// now ss1 has a Derived as resources
  (*ss1).showMe();





}



