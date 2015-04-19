#include "mean.hpp"
#include <iostream>
#include <eigen3/Eigen/Dense>
#include <string>
#include <complex>
int main()
{
    using namespace Eigen;
    using namespace Utility;

    int i = 42;
    std::cout << "mean(7,i):   " << mean(7,i) << std::endl;
    double f1 = 3.4;
    double f2 = -6.7;
    std::cout << "mean(f1,f2): " << mean(f1,f2) << std::endl;
    Matrix2d a;
    a << 1, 2,
         3, 4;
    Matrix2d b(2,2);
    b << 2, 3,
         1, 4;
    std::cout << "mean(a,b): " << mean(a,b) << std::endl;

    std::cout<< "mean('a','z'): "<<mean('a','z')<<std::endl;

    double * ap=new double(5.0);
    double * bp=new double(6.0);
    std::cout<< "mean('ap','bp'): "<<mean(ap,bp)<<std::endl;

    // Now Sum
    std::cout<<Sum(3,4.5,6.7)<<std::endl;
    std::string hello{"Hello"};    
    std::string world{"world"};    
    std::string space{" "};
    std::cout<<Sum(hello,space,world)<<std::endl;
    std::complex<double> one(1.0,0.);
    std::complex<double> im(0.,1.0);
    std::complex<double> biz(1.0,1.0);
    std::cout<<Sum(one,im,biz,10.0)<<std::endl;
}
