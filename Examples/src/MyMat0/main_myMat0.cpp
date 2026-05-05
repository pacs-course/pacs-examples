#include "MyMat0.hpp"
#include "MyMat0_util.hpp"
#include "chrono.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>

int
main()
{
  using namespace LinearAlgebra;
  auto printVector = [](std::vector<double> const &values)
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
  a(0, 0) = 5;
  a(1, 1) = 10;
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
  std::cout << std::endl << std::endl;
  std::cout << "Norm1, NormInf and NormF of a: " << a.norm1() << " "
            << a.normInf() << " " << a.normF() << std::endl;

  // Testing different implementation of matrix/matrix
  constexpr std::size_t NR = 1000;
  constexpr std::size_t NC = 1000;
  {
    // Creating 2 big matrices
    std::cout << "Creating two big matrices ROWMAJOR ROWMAJOR" << std::endl;
    MyMat0<double, ROWMAJOR> A(NR, NC);
    A.fillRandom();
    MyMat0<double, ROWMAJOR> B(NC, NR);
    B.fillRandom();
    std::cout << "Done" << std::endl;
    Timings::Chrono watch;
    std::cout << "Standard Matrix Multiplication"
              << "\n";
    watch.start();
    auto res1 = matMul(A, B);
    watch.stop();
    std::cout << "NormF=" << res1.normF() << std::endl;
    double t1 = watch.wallTime();
    std::cout << watch;
    std::cout << "Optimized Matrix Multiplication"
              << "\n";
    watch.start();
    auto res2 = matMulOpt(A, B);
    watch.stop();
    std::cout << "NormF=" << res2.normF() << std::endl;
    double t2 = watch.wallTime();
    std::cout << watch;
    std::cout << "Gain: " << 100 * (t1 - t2) / t1 << "%" << std::endl;
    std::cout << "Speedup: " << t1 / t2 << std::endl;
#ifndef NOBLAS
    std::cout << " Blas Optimized Matrix Multiplication"
              << "\n";
    watch.start();
    auto res3 = matMulOptBlas(A, B);
    watch.stop();
    std::cout << "NormF=" << res3.normF() << std::endl;
    t2 = watch.wallTime();
    std::cout << watch;
    std::cout << "Gain: " << 100 * (t1 - t2) / t1 << "%" << std::endl;
    std::cout << "Speedup: " << t1 / t2 << std::endl;
#endif
  }
  std::cout << std::endl;
  {
    // Creating 2 big matrices
    std::cout << "Creating two big matrices ROWMAJOR COLUMNMAJOR" << std::endl;
    MyMat0<double, ROWMAJOR> A(NR, NC);
    A.fillRandom();
    MyMat0<double, COLUMNMAJOR> B(NC, NR);
    B.fillRandom();
    std::cout << "Done" << std::endl;
    Timings::Chrono watch;
    std::cout << "Standard Matrix Multiplication"
              << "\n";
    watch.start();
    auto res1 = matMul(A, B);
    watch.stop();
    std::cout << "NormF=" << res1.normF() << std::endl;
    double t1 = watch.wallTime();
    std::cout << watch;
    std::cout << "Optimized Matrix Multiplication"
              << "\n";
    watch.start();
    auto res2 = matMulOpt(A, B);
    watch.stop();
    std::cout << "NormF=" << res2.normF() << std::endl;
    double t2 = watch.wallTime();
    std::cout << watch;
    std::cout << "Gain: " << 100 * (t1 - t2) / t1 << "%" << std::endl;
    std::cout << "Speedup: " << t1 / t2 << std::endl;
#ifndef NOBLAS
    std::cout << " Blas Optimized Matrix Multiplication"
              << "\n";
    watch.start();
    auto res3 = matMulOptBlas(A, B);
    watch.stop();
    std::cout << "NormF=" << res3.normF() << std::endl;
    t2 = watch.wallTime();
    std::cout << watch;
    std::cout << "Gain: " << 100 * (t1 - t2) / t1 << "%" << std::endl;
    std::cout << "Speedup: " << t1 / t2 << std::endl;
#endif
  }
  std::cout << std::endl;
  {
    // Creating 2 big matrices
    std::cout << "Creating two big matrices COLUMNMAJOR COLUMNMAJOR" << std::endl;
    MyMat0<double, COLUMNMAJOR> A(NR, NC);
    A.fillRandom();
    MyMat0<double, COLUMNMAJOR> B(NC, NR);
    B.fillRandom();
    std::cout << "Done" << std::endl;
    Timings::Chrono watch;
    std::cout << "Standard Matrix Multiplication"
              << "\n";
    watch.start();
    auto res1 = matMul(A, B);
    watch.stop();
    std::cout << "NormF=" << res1.normF() << std::endl;
    double t1 = watch.wallTime();
    std::cout << watch;
    std::cout << "Optimized Matrix Multiplication"
              << "\n";
    watch.start();
    auto res2 = matMulOpt(A, B);
    watch.stop();
    std::cout << "NormF=" << res2.normF() << std::endl;
    double t2 = watch.wallTime();
    std::cout << watch;
    std::cout << "Gain: " << 100 * (t1 - t2) / t1 << "%" << std::endl;
    std::cout << "Speedup: " << t1 / t2 << std::endl;
#ifndef NOBLAS
    std::cout << " Blas Optimized Matrix Multiplication"
              << "\n";
    watch.start();
    auto res3 = matMulOptBlas(A, B);
    watch.stop();
    std::cout << "NormF=" << res3.normF() << std::endl;
    t2 = watch.wallTime();
    std::cout << watch;
    std::cout << "Gain: " << 100 * (t1 - t2) / t1 << "%" << std::endl;
    std::cout << "Speedup: " << t1 / t2 << std::endl;
#endif
  }
  std::cout << std::endl;
  {
    // Creating 2 big matrices
    std::cout << "Creating two big matrices COLUMNMAJOR ROWMAJOR" << std::endl;
    MyMat0<double, COLUMNMAJOR> A(NR, NC);
    A.fillRandom();
    MyMat0<double, ROWMAJOR> B(NC, NR);
    B.fillRandom();
    std::cout << "Done" << std::endl;
    Timings::Chrono watch;
    std::cout << "Standard Matrix Multiplication"
              << "\n";
    watch.start();
    auto res1 = matMul(A, B);
    watch.stop();
    std::cout << "NormF=" << res1.normF() << std::endl;
    double t1 = watch.wallTime();
    std::cout << watch;
    std::cout << "Optimized Matrix Multiplication"
              << "\n";
    watch.start();
    auto res2 = matMulOpt(A, B);
    watch.stop();
    std::cout << "NormF=" << res2.normF() << std::endl;
    double t2 = watch.wallTime();
    std::cout << watch;
    std::cout << "Gain: " << 100 * (t1 - t2) / t1 << "%" << std::endl;
    std::cout << "Speedup: " << t1 / t2 << std::endl;
#ifndef NOBLAS
    std::cout << " Blas Optimized Matrix Multiplication"
              << "\n";
    watch.start();
    auto res3 = matMulOptBlas(A, B);
    watch.stop();
    t2 = watch.wallTime();
    std::cout << "NormF=" << res3.normF() << std::endl;
    std::cout << watch;
    std::cout << "Gain: " << 100 * (t1 - t2) / t1 << "%" << std::endl;
    std::cout << "Speedup: " << t1 / t2 << std::endl;
#endif
  }

  /* Only to test if replaceCol works
  std::vector<double> v(A.nrow(),1.0);
  A.replaceCol(3,v);
  auto vv = A.col(3);
  std::cout<<A(0,3)<<" "<<A(A.nrow()-1,3)<<" "<<vv.front()<<"
  "<<vv.back()<<std::endl;
  */
}
