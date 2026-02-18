#ifndef HH_DOMAIN_HH
#define HH_DOMAIN_HH
/*!
  Defines a 1D domain.
 */
namespace Geometry
{
class Domain1D
{
public:
  //! Constructor. Default creates (0,1)
  explicit Domain1D(double a = 0., double b = 1.) noexcept;
  /*! \defgroup Accessor Accessing elements
    @{ */
  double
  left() const noexcept
  {
    return M_a;
  }
  double
  right() const noexcept
  {
    return M_b;
  }
  double &
  left() noexcept
  {
    return M_a;
  }
  double &
  right() noexcept
  {
    return M_b;
  }
  [[nodiscard]] double length() const noexcept;
  /*! @}*/
private:
  double M_a;
  double M_b;
};
} // namespace Geometry
#endif
