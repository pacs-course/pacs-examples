#include <iostream>
#include <vector>
#include <fstream>
#include "MyMat0.hpp"
#include "MyMat0_util.hpp"
#include "chrono.hpp"

int main()
{
  using namespace LinearAlgebra;
  // I create two matrices
  MyMat0<double,COLUMNMAJOR> a;

  a.resize(3,4);
  MyMat0<> b(5,5);

  b.fillRandom();
  a(0,0)=5;
  a(1,1)=10;
  a(2,2)=20;

  //! file to output the matrix
  std::ofstream ofile;
  ofile.open("Matrix.dat");
  std::cout<<"Some output is in the file Matrix.dat"<<std::endl;
  /// I want a certain format and precision
  ofile.setf(std::ios_base::scientific, std::ios_base::floatfield);
  ofile.precision(16);
  std::cout<<"MATRIX a:"<<std::endl;
  a.showMe(); //On screen (default is cout)
  a.showMe(ofile); //Matrix stored on file
  std::cout<<"Matrix b"<<std::endl;
  b.showMe(); 
  b.showMe(ofile); //on file

  ofile.close();// close file

  std::vector<double> va(4,1.0);
  std::vector<double> res(a*va);
  std::cout<<"result of a*[";
  for (auto it=va.cbegin();it<va.cend()-1;
       ++it)std::cout<<*it<<",";
  std::cout<<*(va.end()-1)<<"]^T =";
  std::cout<<"[";
  for (auto i=res.cbegin();i<res.cend()-1;++i)std::cout<<*i<<", ";
  std::cout<<*(res.end()-1)<<"]"<<std::endl<<std::endl;
  std::cout<<"Norm1, NOrmInf and NormF of a: "<<a.norm1()<<" "<<a.normInf()<<" "<<a.normF()<<std::endl;

  // Creating 2 big matrices
  //MyMat0<double,COLUMNMAJOR> A(1000,1000);
  MyMat0<double,ROWMAJOR> A(1024,2048);
  A.fillRandom();
  //  MyMat0<double,COLUMNMAJOR> B(1000,1000);
  MyMat0<double,ROWMAJOR> B(2048,1024);
  B.fillRandom();
  Timings::Chrono watch;
  std::cout<< "Standard Matrix Moltiplication"<<"\n";
  watch.start();
  auto res1=matMul(A,B);
  watch.stop();
  double t1=watch.wallTime();
  std::cout<<watch<<std::endl;
  std::cout<< "Optimized Matrix Moltiplication"<<"\n";
  watch.start();
  auto res2=matMulOpt(A,B);
  watch.stop();
  double t2=watch.wallTime();
  std::cout<<watch<<std::endl;
  std::cout<<"Gain: "<<100*(t1-t2)/t1<<"%"<<std::endl;
  std::cout<<"Speedup: "<<t1/t2<<std::endl;
  std::cout<< " Blas Optimized Matrix Moltiplication"<<"\n";
  watch.start();
  res2=matMulOptBlas(A,B);
  watch.stop();
  t2=watch.wallTime();
  std::cout<<watch<<std::endl;
  std::cout<<"Gain: "<<100*(t1-t2)/t1<<"%"<<std::endl;
  std::cout<<"Speedup: "<<t1/t2<<std::endl;
}
