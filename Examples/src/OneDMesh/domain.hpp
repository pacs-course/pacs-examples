#ifndef HH_DOMAIN_HH
#define HH_DOMAIN_HH
/*!
 * @file domain.hpp
 * @brief Definition of a simple one-dimensional domain.
 */
namespace Geometry
{
/*!
 * @brief Closed interval used as computational domain for 1D meshes.
 *
 * The class stores the two endpoints of a one-dimensional interval and offers
 * lightweight accessors together with a convenience method for its length.
 *
 * @note The class does not enforce any ordering between the endpoints.
 * The interval orientation is therefore left to the calling code.
 */
class Domain1D
{
public:
  /*!
   * @brief Construct a one-dimensional domain.
   * @param a Left endpoint.
   * @param b Right endpoint.
   *
   * The default domain is the unit interval `[0,1]`.
   */
  explicit Domain1D(double a = 0., double b = 1.) noexcept;
  /*!
   * @name Endpoint Access
   * @{
   */
  //! @brief Read-only access to the left endpoint.
  double
  left() const noexcept
  {
    return M_a;
  }
  //! @brief Read-only access to the right endpoint.
  double
  right() const noexcept
  {
    return M_b;
  }
  //! @brief Writable access to the left endpoint.
  double &
  left() noexcept
  {
    return M_a;
  }
  //! @brief Writable access to the right endpoint.
  double &
  right() noexcept
  {
    return M_b;
  }
  /*!
   * @brief Length of the interval.
   * @return `|right()-left()|`.
   */
  [[nodiscard]] double length() const noexcept;
  /*! @} */
private:
  //! Left endpoint.
  double M_a;
  //! Right endpoint.
  double M_b;
};
} // namespace Geometry
#endif
