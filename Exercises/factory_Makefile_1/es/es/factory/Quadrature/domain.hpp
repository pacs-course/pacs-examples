#ifndef _HH_DOMAIN_HH
#define _HH_DOMAIN_HH
namespace Geometry{
  class Domain1D
  {
  public:
    Domain1D(double const & a, double const & b);
    inline double left(){return _a;}
    inline double right(){return _b;}
  private:
    double _a;
    double _b;
  };
}
#endif
