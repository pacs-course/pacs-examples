#include "meshGenerators.hpp"
#include <iostream>
#include <fstream>
#include <cmath>
int main()
{
  using namespace Generator;
  using namespace Geometry;
  Domain1D domain(0.,10.);
  const double pi=std::atan(1.0)*4.0;
  // Uniform mesh
  Uniform uniformMeshGenerator(domain,100);
  auto meshNodesUniform=uniformMeshGenerator();
  std::ofstream out1("uniform.dat");
  for (auto i : meshNodesUniform)
    {
      out1<<i<<" 0"<<std::endl;
    }
  out1.close();
  //auto h = [](double const & x){return 0.01+0.005*x*x;};
  auto h = [pi](double const & x){return (0.05+ 0.1*std::sin(x*pi/10.));};
  //auto h = [pi](double const & x){return (0.05+ 100*x);};
  VariableSize variableMeshGenerator(domain, h,10000);
  auto meshNodesVariable=variableMeshGenerator();

  std::ofstream out2("variable.dat");
  for (auto i : meshNodesVariable)
    {
      out2<<i<<" 0"<<std::endl;
    }
  out2.close();
  
}
