#ifndef HH_VECTOREXPR_HPP
#define HH_VECTOREXPR_HPP
#include "expressionWrapper23.hpp"
#include <concepts>
#include <utility>
#include <vector>
namespace ET23
{
//! An owning vector of double values that also acts as an expression leaf.
/*!
  M_data is the only instance data member. Copying a Vector copies its elements;
  expression construction and assignment evaluate lazily described arithmetic
  into this storage. Inheriting from the non-template Expr enables the
  std::derived_from constraint; evaluation uses Vector's concrete members.
 */
class Vector : public Expr
{
  std::vector<double> M_data;

public:
  //! Construct an empty vector.
  Vector() = default;
  //! Explicitly copy the elements of an lvalue standard container.
  explicit Vector(std::vector<double> const &v) : M_data{v} {};
  //! Explicitly move storage from an rvalue standard container.
  explicit Vector(std::vector<double> &&v) : M_data{std::move(v)} {};
  //! Construct n elements initialized to value (zero by default).
  Vector(std::size_t n, double value = 0.0) : M_data(n, value) {};
  //! Copy elements into independent storage.
  Vector(Vector const &) = default;
  //! Move storage; the source remains valid with unspecified contents.
  Vector(Vector &&) = default;
  //! Copy elements from another Vector.
  Vector &operator=(Vector const &) = default;
  //! Move storage from another Vector.
  Vector &operator=(Vector &&) = default;
  //! Materialize an expression into an owning vector.
  /*!
    Preserve the concrete expression type: const Expr& would select the
    nonvirtual base fallbacks. Ordinary copy/move overloads remain preferred
    for Vector arguments. Every referenced operand must remain alive and have
    a stable size throughout evaluation; binary vector operands must agree
    in size. No intermediate result vectors are constructed by this loop.
   */
  template <std::derived_from<Expr> Expression>
  Vector(Expression const &et) : M_data()
  {
    std::size_t const n = et.size();
    M_data.reserve(n);
    for(std::size_t i = 0; i < n; ++i)
      M_data.emplace_back(et[i]);
  }
  //! Resize the destination and evaluate each element of an expression.
  /*!
    The same lifetime and size preconditions as expression construction apply.
    Same-size pointwise self-assignment (v = -v or v = v + w) is supported:
    each right-hand element is read before the corresponding write. There is
    no general alias detection. Expressions whose reads depend on other output
    indices, or whose operands are invalidated by resizing, need a temporary.
    If element evaluation throws, earlier destination elements may be changed.
   */
  template <std::derived_from<Expr> Expression>
  Vector &
  operator=(Expression const &et)
  {
    std::size_t const n = et.size();
    M_data.resize(n);
    for(std::size_t i = 0; i < n; ++i)
      M_data[i] = et[i];
    return *this;
  }
  //! Return a mutable reference to element i; requires i < size().
  double &
  operator[](std::size_t i)
  {
    return M_data[i];
  }
  //! Read element i by value; requires i < size().
  double
  operator[](std::size_t i) const
  {
    return M_data[i];
  }
  //! Number of stored elements.
  std::size_t
  size() const
  {
    return M_data.size();
  }
  //! Expose the owned container by const reference, without copying.
  // The returned reference must not outlive this Vector.
  operator std::vector<double> const &() const { return M_data; }
  //! Expose the owned container by mutable reference, without copying.
  // Container operations may invalidate element references and iterators.
  operator std::vector<double> &() { return M_data; }
  //! Named alternative to the const container conversion.
  std::vector<double> const &
  as_vector() const
  {
    return M_data;
  }
  //! Expose the owned container by mutable reference, without copying.
  // Container operations may invalidate element references and iterators.
  std::vector<double> &
  as_vector()
  {
    return M_data;
  }
};

//! Begin mutable iteration; found by argument-dependent lookup in range-for.
// Const Vector iteration uses cbegin/cend or the const as_vector() accessor;
// this example does not supply begin/end overloads for const Vector.
inline auto
begin(Vector &a)
{
  return static_cast<std::vector<double> &>(a).begin();
}

//! End mutable iteration.
inline auto
end(Vector &a)
{
  return static_cast<std::vector<double> &>(a).end();
}

//! Begin read-only iteration over a const or mutable Vector.
inline auto
cbegin(Vector const &a)
{
  return static_cast<std::vector<double> const &>(a).cbegin();
}

//! End read-only iteration over a const or mutable Vector.
inline auto
cend(Vector const &a)
{
  return static_cast<std::vector<double> const &>(a).cend();
}
} // namespace ET23
#endif
