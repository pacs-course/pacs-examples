#ifndef HH_VECTOREXPR_HPP
#define HH_VECTOREXPR_HPP
#include "expressionWrapper.hpp"
#include <utility>
#include <vector>
namespace ET
{
//! A class for vectors of double
/*!
  It is build around std::vector<double> and indeed
  std::vector:double> is the only variable member of the class.
 */
class Vector : public Expr<Vector>
{
  std::vector<double> M_data;

public:
  //! default constructor
  Vector() = default;
  //! A vector may be converted to a Vector
  explicit Vector(std::vector<double> const &v) : M_data{v} {};
  //! A vector may also be moved in a Vector
  explicit Vector(std::vector<double> &&v) : M_data{std::move(v)} {};
  //! Construct a Vector of n elements initialised by value
  Vector(unsigned int n, double value=0.0) : M_data(n, value){};
  //! Copy constructor
  Vector(Vector const &) = default;
  //! Move constructor
  Vector(Vector &&) = default;
  //! Copy assign
  Vector &operator=(Vector const &) = default;
  //! MOve assign
  Vector &operator=(Vector &&) = default;
  //! I may build a Vector from an expression!
  template <class T> Vector(const Expr<T> &e) : M_data()
  {
    const T &et(e); // casting!
    M_data.reserve(et.size());
    for(auto i = 0u; i < et.size(); ++i)
      M_data.emplace_back(et[i]);
  }
  //! Assigning an expression
  /*!
    This method is fundamental for expression template technique.
   */
  template <class T>
  Vector &
  operator=(const Expr<T> &e)
  {
    const T &et(e); // casting!
    M_data.resize(et.size());
    for(auto i = 0u; i < et.size(); ++i)
      M_data[i] = et[i];
    return *this;
  }
  //! Returns i-th element
  double &
  operator[](std::size_t i)
  {
    return M_data[i];
  }
  //! Returns i-th element (const version)
  double
  operator[](std::size_t i) const
  {
    return M_data[i];
  }
  //! size of the vector
  std::size_t
  size() const
  {
    return M_data.size();
  }
  //! May be cast to a std::vector &
  /*!
    This way I can use all the methods of a std::vector!
    @code
    Vector a;
    std::vector<double> & av(a); // you cannot use {a} here!
    av.emplace_back(10.0);
    @endcode

   */
  operator std::vector<double> const &() const { return M_data; }
  //! Non const version of casting operator
  operator std::vector<double> &() { return M_data; }
  //! This does the same as casting but with a method
  /*!
    Only to show that you do not need casting operators, if you find them
    confusing (or if the compiler gets confused!)
    @code
    Vector a;
    std::vector<double> & av{a.as_vector()}; // Here you may use {}! (but ()
    works as well!) av.emplace_back(10.0);
    @endcode

  */
  std::vector<double> const &
  as_vector() const
  {
    return M_data;
  }
  //! Non const version of casting operator
  std::vector<double> &
  as_vector()
  {
    return M_data;
  }
};

//! I want to use range for loops with Vector objects.
/*!
  Note the use of declval. I do not need to istantiate a vector to interrogate
  the type returned by begin!
 */
inline auto
begin(Vector &a) -> decltype(std::declval<std::vector<double> >().begin())
{
  // I exploit the fact tha I have a casting operator to std::vector<double>&
  return static_cast<std::vector<double> &>(a).begin();
  // If you prefer
  // return a.as_vector().begin();
}

inline auto
end(Vector &a) -> decltype(std::declval<std::vector<double> >().end())
{
  // I exploit the fact tha I have a casting operator to std::vector<double>&
  return static_cast<std::vector<double> &>(a).end();
}

inline auto
cbegin(Vector const &a)
  -> decltype(std::declval<std::vector<double> >().cbegin())
{
  // I exploit the fact tha I have a casting operator to std::vector<double>
  // const &
  return static_cast<std::vector<double> const &>(a).cbegin();
}

inline auto
cend(Vector const &a) -> decltype(std::declval<std::vector<double> >().cend())
{
  // I exploit the fact tha I have a casting operator to std::vector<double>
  // const &
  return static_cast<std::vector<double> const &>(a).cend();
}
} // namespace ET
#endif
