 /*
 * Accelerators.hpp
 *
 *  Created on: Jan 25, 2020
 *      Author: forma
 */

#ifndef SRC_NONLINSYSSOLVER_ACCELERATORS_HPP_
#define SRC_NONLINSYSSOLVER_ACCELERATORS_HPP_
#include "FixedPointTraits.hpp"
#include "VectorInterface.hpp"
#include <cmath>
#include <functional>
#include <memory>
//! \file Accelerators.hpp Different accelerators for fixed point iteration
namespace apsc
{
//! An enum that stores the different options made available
enum FPAcceleratorId
{
  NoAcceleration,
  ASecant
};

//! The simple iterator
/*!
 * This class is very thin since it only defines a call operator that
 * given a new iterate returns the application of the iteration function. This
 * class is called noAcceleration, since it just implements the simple fixed
 * point iteration step
 *
 * \tparam IteratorFunction. The iterator function. Note that at this level the
 * iterator function is used only as a trait to extract the type used for the
 * arguments. It must conform to a std::function!
 *
 */
template <FixedPointArgumentType ARG>
class NoAccelerator
{
public:
  using ArgumentType = typename FixedPointTraits<ARG>::ArgumentType;
  using IterationFunction = typename FixedPointTraits<ARG>::IterationFunction;
  NoAccelerator(IterationFunction const &I) : M_I{I} {}; //!< Does nothing
  //! Call operator that returns the accelerated iterate
  ArgumentType
  operator()(ArgumentType const &x)
  {
    return M_I(x);
  }
  //! Puts the accelerator to the original setting
  void reset(){};
  //! Returns the identifier of the accelerator. Useful to implement different
  //! choices run-rime
  constexpr FPAcceleratorId
  getId()
  {
    return NoAcceleration;
  }
  //! The identifier is an attribute of the class. It may be tested on the type.
  static constexpr FPAcceleratorId id = NoAcceleration;

protected:
  //!@note I decided to store the function (composition). An alternative is
  //!aggregation (useful if IterationFunction is a big object)
  const IterationFunction &M_I;
};

//! Alternate secant method
/*!
 * This is a two-level acceleration method equivalent to a two level Anderson
 * method
 * \f[
 * x_{n+1}=\phi(x_n)-\frac{(\Delta x_n -\Delta x_{n-1})\cdot\Delta
 * x_n}{||\Delta x_n -\Delta x_{n-1}||^2}(\phi(x_n)-\phi(x_{n-1})
 * \f]
 *
 * V. Eyert, A Comparative Study on Methods for Convergence Acceleration of
 * Iterative Vector Sequences, Journal of Computational Physics 124 (1996)
 * 271–285. H. Fang, Y. Saad, Two classes of multisecant methods for nonlinear
 * accel- eration, Numerical Linear Algebra with Applications 16 (2009) 197–221.
 *
 * \tparam IteratorFunction. The iterator function. Note that at this level the
 * iterator function is used only as a trait to extract the type used for the
 * arguments. No other requirements are made
 *
 */
template <FixedPointArgumentType ARG>
class ASecantAccelerator
{
public:
  using ArgumentType = typename FixedPointTraits<ARG>::ArgumentType;
  using IterationFunction = typename FixedPointTraits<ARG>::IterationFunction;
  ASecantAccelerator(IterationFunction const &I) : M_I{I} {}; //!< Does nothing
  //! Call operator that returns the accelerated iterate
  ArgumentType operator()(ArgumentType const &x);
  //! Put back the accelerator to initial setting
  void
  reset()
  {
    length = 0;
    firstTime = true;
  }
  //! Returns the identifier of the accelerator. Useful to implement different
  //! choices run-rime
  constexpr FPAcceleratorId
  getId()
  {
    return id;
  }
  //! The identifier is an attribute of the class. It may be tested on the type.
  static constexpr FPAcceleratorId id = ASecant;

protected:
  const IterationFunction &M_I;
  ArgumentType             deltaXOld;
  ArgumentType             xOld;
  ArgumentType             phiOld;
  size_t                   length{0};
  ArgumentType             xNew;
  ArgumentType             deltaXNew;
  bool                     firstTime = true;
};

template <FixedPointArgumentType ARG>
typename ASecantAccelerator<ARG>::ArgumentType
ASecantAccelerator<ARG>::operator()(ArgumentType const &x)
{
  using namespace apsc::internals;
  if(firstTime)
    {
      // set initial values
      this->length = x.size();
      this->xNew = this->M_I(x);
      this->phiOld = xNew;
      deltaXOld.resize(length);
      deltaXNew.resize(length);
      // compute \Delta x_n = phi(x_n)-x_n
      this->deltaXOld = xNew - x;
      firstTime = false;
    }
  else
    {
      this->xNew = this->M_I(x);
      // compute \Delta x_n = phi(x_n)-x_n
      deltaXNew = xNew - x;
      ArgumentType tmp(length);
      // \Delta x_n - \Delta x_{n-1}
      tmp = deltaXNew - deltaXOld;
      // Get ||\Delta x_n - \Delta x_{n-1}||^2
      double norm2 = squaredNorm(tmp);
      // to do: check norm2
      norm2 = 1.0 / norm2;
      ArgumentType tmp2(length);
      //! Scaling:  \Delta x_n - \Delta x_{n-1}/||\Delta x_n - \Delta
      //! x_{n-1}||^2
      tmp2 = norm2 * tmp;
      //! Do the dot product with \Delta x_n
      double factor = dot(tmp2, deltaXNew);
      // Save phi(X_n) for later use
      tmp2 = xNew;
      // scale (phi(x_n)-\phi(x_{n-1}) by factor and subtract
      //
      xNew = xNew - factor * (xNew - phiOld);
      phiOld = tmp2;
      deltaXOld = deltaXNew;
    }
  return xNew;
}
} // end namespace apsc

#endif /* SRC_NONLINSYSSOLVER_ACCELERATORS_HPP_ */
