#include "functors.hpp"
#include <cmath>
namespace myfunctors
{
  double Sqrt5::operator()(double a) const
  {
    //y=sqrt5(a)
    //y^5-a=0
    //df =5*y^4
    if (a==0.0) return 0.0;
    auto der=[](double const x){return 5*x*x*x*x;};
    auto resid=[&a](double const x){return x*x*x*x*x-a;};
    auto err=2*tolerance;
    unsigned int iter   = 0;
    auto x = x0;
    while ((err>tolerance) && (iter <maxiter))
      {
        // I should check if der=0 but here I avoid for simplicity
        auto step = -resid(x)/der(x);
        err = std::abs(step);
        x+=step;
        ++iter;
      }
    return x;
  }
  
  double Cross::operator()(Cross::Vector2D const & a, Cross::Vector2D const & b)const
  {
    return a[0]*b[1]-a[1]*b[0];
  }
  
  Cross::Vector3D Cross::operator()(Cross::Vector3D const & a, Cross::Vector3D const & b)const
  {
    // ! call the constructor of array directly and return the temporary.
    return {a[1]*b[2]-a[2]*b[1], b[0]*a[2]-a[0]*b[2],a[0]*b[1]-a[1]*b[0]};
  }

  double Cross::operator()(Cross::Vector3D const & a, Cross::Vector3D const & b,Cross::Vector3D const & c)const
  {
    auto v = this->operator()(b,c);
    return a[0]*v[0]+a[1]*v[1]+a[2]*v[2];
  }
  
}
