#ifndef HH_DOMAIN_HH
#define HH_DOMAIN_HH
/*!
  Defines a 1D domain.
 */
namespace Geometry{
  class Domain1D
  {
  public:
    //!Constructor. Default creates (0,1)
    explicit Domain1D(double const & a=0., double const & b=1.);
    /*! \defgroup Accessor Accessing elements
      @{ */
    double left()const {return _a;}
    double right()const {return _b;}
    double & left(){return _a;}
    double & right(){return _b;}
    /*! @}*/
  private:
    double _a;
    double _b;
  };
}
#endif
