#ifndef HH_EXPRESSIONWRAPPER23_HH
#define HH_EXPRESSIONWRAPPER23_HH
#include <cstddef>
#include <utility>
#include <stdexcept>
namespace ET23
{
//! Common, non-template base for the expression types.
/*!
  Concrete types inherit from Expr, rather than from Expr<Derived> as in CRTP.
  Each concrete type supplies public size() and operator[] members. Vector's
  constrained evaluation templates preserve that concrete type and call its
  members directly; the inherited members below are hidden on this path.

  The protected forwarding templates illustrate C++23 explicit object
  parameters ("deducing this"). A qualified call from a derived class can
  deduce the receiver's static type without a CRTP downcast. This is static
  dispatch: converting an expression to Expr const& loses its concrete type.
  Explicit object parameters do not recover a runtime derived type.

  The public fallback members are teaching scaffolding, not a type-erased
  evaluation interface. An Expr alone represents no stored vector data.
 */
struct Expr
{
protected:
  //! Forward size lookup using the deduced static type of the receiver.
  //! The concrete type must supply size() to terminate this delegation.
  std::size_t
  size(this auto const &self)
  {
    return self.size();
  }

  //! Read an element through the concrete receiver's const indexing member.
  //! \pre i is a valid index for the concrete expression.
  template <typename Self>
  double
  operator[](this Self const &self, std::size_t i)
  {
    return self.operator[](i);
  }

  //! Preserve the concrete indexing result and receiver's cv/ref category.
  /*!
    A mutable Vector returns double&, while arithmetic nodes return double.
    decltype(auto) supports both; an unconditional double& would be ill-formed
    when forwarding to a node that computes a value. Forwarding does not extend
    the receiver's lifetime, so a reference into a temporary Vector can dangle.
   */
  template <typename Self>
  decltype(auto)
  operator[](this Self &&self, std::size_t i)
  {
    return std::forward<Self>(self).operator[](i);
  }

  //! Shared dummy storage used only by the mutable base fallback.
  static inline double d{0.};

public:
  //! Base fallback: you should neve instantiete Expr directly only classes derived from it
  std::size_t size() const { throw std::runtime_error("Invalid access to expression (have you used Expr directly?)"); }

  //! Base fallback: you should neve instantiete Expr directly only classes derived from it
  double operator[]([[maybe_unused]] std::size_t i) const { throw std::runtime_error("Invalid access to expression (have you used Expr directly?)"); }

  //! Base fallback: you should neve instantiete Expr directly only classes derived from it
  double &operator[]([[maybe_unused]] std::size_t i) { throw std::runtime_error("Invalid access to expression (have you used Expr directly?)"); }
};
} // namespace ET23
#endif
