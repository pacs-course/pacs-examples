#include <iostream>
#include "biforc.hpp"
int main ()
{
  using namespace std;
  using namespace Darcy;
  using namespace Geometry;
  
  Point e0(0.,-1.);
  Point e1(-1.,0.);
  Point e2(1.,0.);
  Point ip(0.,0.);
  FractureEnd f0{e0,0.1};
  FractureEnd f1{e1,0.1};
  FractureEnd f2{e2,0.05};
  Intersection intersection(f0,f1,f2,ip);
  Triangle triangle=intersection.computeIntersectionTriangle();
  std::cout<<triangle<<std::endl;

  Matrix2d permeability;

  //  permeability<<2500.3,2499.8, 
  //            2499.8, 2500.3;
  //  permeability<<2500.0,0.0, 
  //            0.0, 0.2;
  permeability<<1000.0,0.0, 
                0.0, 1.0;
  cout<<"**** Eigenvalues of permeability matrix"<<endl;
  cout<<permeability.eigenvalues()<<endl;

  Point p0(0.0,0.0);
  Point p1(1.0,0.0);
  Point p2(0,1.0);

  // triangle.setPoint(0,p0);
  //triangle.setPoint(1,p1);
  //triangle.setPoint(2,p2);

  
  Biforc biforc(permeability);
  biforc.setTriangle(triangle);
  biforc.computeT();
  cout<<" ************ Permeability **************"<<endl;
  cout<<biforc.K()<<endl;
  cout<<" ************ Matrix C     **************"<<endl;
  cout<<biforc.C()<<endl;
  cout<<" ************ Matrix N **************"<<endl;
  cout<<biforc.N()<<endl;
  cout<<" ************ Matrix Qc **************"<<endl;
  cout<<biforc.Qc()<<endl;
  cout<<" ************ Matrix Pc **************"<<endl;
  cout<<biforc.Pc()<<endl;
  cout<<" ************ Matrix T  **************"<<endl;
  cout<<biforc.T()<<endl; 
  cout<<" ************eigenvalues  **************"<<endl;
  cout<<biforc.T().eigenvalues()<<endl; 
  cout<<" ************pressure coeff  **************"<<endl;
  cout<<pressureCoeff(biforc.T())<<endl; 

  
}
