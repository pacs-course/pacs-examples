#ifndef HH__FUNCTORS_HH
#define HH__FUNCTORS_HH
#include <array>
#include <functional> // for std function objects (like less<>)
#include <string>
#include <cmath>
#include <stdexcept>
// Some examples of functors (also called function object)
/*
  A function object is any object of class type that defines the call operator.
  They behave very similar to a function but they may have a state
  (variable members) and possibly other methods as well, so they may be more
  flexible.

  Moreover termplate functors allow partial specialization, which is not allowed
  in normal template functions. This may be useful for some metaprogramming
  techniques.
 */
namespace myfunctors
{
/*!
 *
 * @brief Computes the fitfh root of a number by Newton method
 * 
 * A functor (object function) that takes a single argument is also called aunary functor.
 * This object function computes fifth root of a by Newton method up to 10^-6 precision. Definition in
 * the cpp file
 */
struct Sqrt5
{
  double operator()(double a) const;
  //! max number of iterations (default=100)
  unsigned int maxiter = 100;
  //! Absolute tolerance
  double tolerance = 1.e-6;
  //! Initial value.
  double x0 = 1.0;
};
/*!
@brief Computes the N-th root of a number by Newton method
 *
 * This object function computes N-th root of a by Newton method up to 10^-6 precision. Definition in
 * the header file since it is a templates
 * @tparam p the root to compute
 * @param a the number to compute the root of
 * @return the N-th root of a by Newton method
 
*/
template <unsigned int p>
struct rootN
{
  double operator()(double a) const
  {
    static_assert(p > 0, "rootN: p must be positive");
    if(a == 0.0)
      return 0.0;
        //auto         phi =[&a](double const x) { return (a/x/x/x/x -x ) / 5; };

    constexpr int exp = 1u - p; // the exponent in the Newton iterations
    auto         err = 2 * tolerance;
    unsigned int iter = 0;
    auto         x = x0;
    while((err > tolerance) && (iter < maxiter))
      {
        auto step = (a*std::pow(x,exp) -x ) / p;
        err = std::abs(step);
        x += step;
        ++iter;
      }
    // This most complex version throw an exception if the number of iterations exceeds maxiter
    if(iter == maxiter)
      throw std::runtime_error("RootN: Too many iterations");
    return x;
  }
  //! max number of iterations (default=100)
  unsigned int maxiter = 100;
  //! Absolute tolerance
  double tolerance = 1.e-6;
  //! Initial value.
  double x0 = 1.0;
};
//! A predicate is a function or functor that defines a call operator
//! returning a bool.
/*!
  Here I show why the state can be useful. Here I have a functor
  that returns true if the value passed is greater than a value that
  may be changed run time.

*/
class Isgreater
{
public:
  Isgreater(double const &a) : my_value{a} {};
  bool
  operator()(double const &b) const
  {
    return b > this->my_value; // this-> can be omitted
  }
  double my_value;
};
//! A binary function takes 2 arguments.
/*! A comparison operator is a binary function that returns a bool and
  is consistent with an ordering relation.
  If you inherit publicly for binary_function (not strictly
  necessary) you inherit three typedefs first_argument_type,
  second_argument_type and result_type.
*/
struct LessModulo10
{
  //! the call operator implementing the operation
  /*!  Definition in-class (thus in the header file) because it is a
    very short function and may be inlined (in-class methods are
    implicitely declared inline, you do not need to specify the
    inline keyword).
  */
  constexpr bool
  operator()(int a, int b) const
  {
    return a % 10 < b % 10;
  }
};
//! A more complex functor: cross product
/*!

   To show that you may have functors with more, overloaded, call
  operators and operators that takes more than 2 arguments.  It is
  only an example.

  \note A note, all those operators could have been declared static
  since they do not use non-static member variables (indeed there
  are no member variables!).

*/
struct Cross
{
  //! 2D vectors
  using Vector2D = std::array<double, 2>;
  //! 3D vectors
  using Vector3D = std::array<double, 3>;
  //! Computes cross products of 2D Vectors
  double operator()(Vector2D const &a, Vector2D const &b) const;
  //! Computes cross products of 3D Vectors
  Vector3D operator()(Vector3D const &a, Vector3D const &b) const;
  //! Computes  \f$ a\cdot b\times c\f$
  double operator()(Vector3D const &a, Vector3D const &b,
                    Vector3D const &c) const;
};
} // namespace myfunctors
#endif
