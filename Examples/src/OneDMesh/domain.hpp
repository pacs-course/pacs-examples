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
    double left()const {return M_a;}
    double right()const {return M_b;}
    double & left(){return M_a;}
    double & right(){return M_b;}
    double length()const;
    /*! @}*/
  private:
    double M_a;
    double M_b;
  };
}
#endif
