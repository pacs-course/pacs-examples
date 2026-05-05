#include "MyMat0.hpp"
#include "MyMat0_views.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>
struct Foo
{
  double a;
  int    b;
};

int
main()
{
  using namespace LinearAlgebra;
  auto printVector = [](auto const &values)
  {
    std::cout << "[";
    for(std::size_t i = 0; i < values.size(); ++i)
      {
        if(i != 0)
          std::cout << ", ";
        std::cout << values[i];
      }
    std::cout << "]";
  };
  // I create two matrices
  MyMat0<double, COLUMNMAJOR> a;

  a.resize(3, 4);
  MyMat0<> b(5, 5);

  b.fillRandom();
  a(0, 0) = 5.5;
  a(1, 1) = 10.2;
  a(2, 2) = 20;

  //! file to output the matrix
  std::ofstream ofile{"Matrix.dat"};
  if(!ofile)
    throw std::runtime_error("Unable to open Matrix.dat for writing");
  std::cout << "Some output is in the file Matrix.dat" << std::endl;
  /// I want a certain format and precision
  ofile.setf(std::ios_base::scientific, std::ios_base::floatfield);
  ofile.precision(16);
  std::cout << "MATRIX a:" << std::endl;
  a.showMe();      // On screen (default is cout)
  a.showMe(ofile); // Matrix stored on file
  std::cout << "Matrix b" << std::endl;
  b.showMe();
  b.showMe(ofile); // on file

  ofile.close(); // close file

  std::vector<double> va(4, 1.0);
  std::vector<double> res(a * va);
  std::cout << "Result of a*";
  printVector(va);
  std::cout << "^T = ";
  printVector(res);
  std::cout << std::endl;
  std::cout << "Norm1, NormInf and NormF of a: " << a.norm1() << " "
            << a.normInf() << " " << a.normF() << std::endl;

  // A matrix of integers
  MyMat0<int> aInt(2, 2);
  aInt(0, 0) = 1;
  aInt(0, 1) = 2;
  aInt(1, 0) = 3;
  aInt(1, 1) = 4;
  aInt.showMe();
  std::cout << "Norm1, NormInf and NormF of aInt: " << aInt.norm1() << " "
            << aInt.normInf() << " " << aInt.normF() << std::endl;
  TransposeView<MyMat0<int>> aIntT{aInt};
  std::cout << "Transpose:" << std::endl;
  aIntT.showMe();
  aInt(0, 0) = 10;
  TransposeView<const MyMat0<int>> c_aIntT{aInt};
  auto const                       z = c_aIntT(0, 1);
  std::cout << "Entry (0,1) of the const transpose view: " << z << std::endl;

  std::cout << "Norm1, NormInf and NormF of aIntT: " << aIntT.norm1() << " "
            << aIntT.normInf() << " " << aIntT.normF() << std::endl;

  // Matrix-vector product through the transpose view
  auto res2 = aIntT * std::vector<int>{1, 2};
  std::cout << "aIntT * ";
  printVector(std::vector<int>{1, 2});
  std::cout << " = ";
  printVector(res2);
  std::cout << std::endl;

  // A matrix of Foos
  MyMat0<Foo, ROWMAJOR> fooMat(2, 2);
  fooMat(0, 0) = {1.0, 2};
  fooMat(0, 1) = {2.0, 1};
  fooMat(1, 0) = {3.0, 0};
  fooMat(1, 1) = {4.0, -1};
  std::cout << "fooMat(1,1)={" << fooMat(1, 1).a << " " << fooMat(1, 1).b << "}"
            << std::endl;
  // Try to uncomment this!
  // fooMat.normInf();
  // or this
  // fooMat.norm1();
}
