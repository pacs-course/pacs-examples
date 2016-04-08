#include <iostream>
#include <vector>
#include <fstream>
#include "MyMat0.hpp"
#include "MyMat0_views.hpp"
struct Foo
{
  double a;
  int b;
};
  

int main()
{
  using namespace LinearAlgebra;
  // I create two matrices
  MyMat0<double,COLUMNMAJOR> a;

  a.resize(3,4);
  MyMat0<> b(5,5);

  b.fillRandom();
  a(0,0)=5.5;
  a(1,1)=10.2;
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
  std::cout<<*(res.end()-1)<<"]"<<std::endl;
  std::cout<<"Norm1, NormInf and NormF of a: "<<a.norm1()<<" "<<a.normInf()<<" "<<a.normF()<<std::endl;

  // A matrix of integers
  MyMat0<int> aInt(2,2);
  aInt(0,0)=1;
  aInt(0,1)=2;
  aInt(1,0)=3;
  aInt(1,1)=4;
  aInt.showMe();
  std::cout<<"Norm1, NormInf and NormF of aInt: "<<aInt.norm1()<<" "<<aInt.normInf()<<" "<<aInt.normF()<<std::endl;
  DiagonalView<MyMat0<int>> aIntT{aInt};
  std::cout<<"Transpose:"<<std::endl;
  aIntT.showMe();  
  std::cout<<"Norm1, NormInf and NormF of aIntT: "<<aIntT.norm1()<<" "<<aIntT.normInf()<<" "<<aIntT.normF()<<std::endl;
  
  // Just to test that it compiles fine
  auto res2= aIntT*std::vector<int>{1,2};
  
  // A matrix of Foos
  MyMat0<Foo,ROWMAJOR> fooMat(2,2);
  fooMat(0,0)={1.0,2};
  fooMat(0,1)={2.0,1};
  fooMat(1,0)={3.0,0};
  fooMat(1,1)={4.0,-1};
  std::cout<<"fooMat(1,1)={"<<fooMat(1,1).a<<" "<<fooMat(1,1).b<<"}"<<std::endl;
  // Try to uncomment this!
  //fooMat.normInf();
  // or this
  //fooMat.norm1();

}
