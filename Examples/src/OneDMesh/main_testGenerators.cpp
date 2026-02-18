#include "mesh.hpp"
#include <cmath>
#include <fstream>
#include <iostream>
#include <numbers>
#include <stdexcept>
int
main()
{
  using namespace Geometry;
  Domain1D     domain(0., 10.);
  constexpr double pi = std::numbers::pi_v<double>;
  // Uniform mesh
  std::size_t N = 100u;
  std::cout << "Creating a uniform mesh with " << N << "elements\n";
  Mesh1D uniformMesh(domain, N);
  std::cout << "Number of nodes created: " << uniformMesh.numNodes()
            << " with max size=" << uniformMesh.hmax()
            << " and min size=" << uniformMesh.hmin() << std::endl;
  std::cout << "Storing it in uniform.dat\n";
  std::ofstream out1("uniform.dat");
  if(!out1)
    throw std::runtime_error("Cannot open output file uniform.dat");
  for(auto i : uniformMesh)
    {
      out1 << i << " 0" << std::endl;
    }
  out1.close();
  // auto h = [](double const & x){return 0.01+0.005*x*x;};
  auto h = [pi](double x) {
    return (0.05 + std::abs(0.1 * std::sin(x * pi / 10.)));
  };
  // auto h = [pi](double const & x){return (0.05+ 100*x);};
  N = 10000u;
  std::cout << "Creating a non uniform mesh with max " << N << " elements\n";
  Geometry::VariableSize vmg(domain, h, N);
  Mesh1D                 variableSizeMesh(vmg);
  std::cout << "Number of nodes created: " << variableSizeMesh.numNodes();
  std::cout << " with max size=" << variableSizeMesh.hmax()
            << " and min size=" << variableSizeMesh.hmin() << std::endl;
  std::cout << "Storing it in variable.dat\n";
  std::ofstream out2;
  out2.open("variable.dat");
  if(!out2)
    throw std::runtime_error("Cannot open output file variable.dat");
  for(auto i : variableSizeMesh)
    {
      out2 << i << " 0" << std::endl;
    }
  out2.close();
  std::cout << "You may plot the mesh using gnuplot\n and launching the "
               "command plot \"filename\""
            << std::endl;
}
