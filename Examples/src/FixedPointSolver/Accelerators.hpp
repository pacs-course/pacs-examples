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
#include <deque>
#include <functional>
#include <memory>
//! \file Accelerators.hpp Different accelerators for fixed point iteration
namespace apsc
{
//! An enum that stores the different options made available
enum FPAcceleratorId
{
  None,
  ASecant,
  Anderson
};

//! The simple iterator
/*!
 * This class is very thin since it only defines a call operator that
 * given a new iterate returns the application of the iteration function. This
 * class is called noAcceleration, since it just implements the simple fixed
 * point iteration step.
 *
 * It is also the base class for all the other accelerators
 *
 * \tparam ARG. The iterator function.
 * It must be equal to the type of the argument of the fixed point solver
 * possible values are `FixedPointArgumentType::VECTOR` and
 * `FixedPointArgumentType::EIGEN`
 *
 */
template <FixedPointArgumentType ARG> class AcceleratorBase
{
public:
  using ArgumentType = typename FixedPointTraits<ARG>::ArgumentType;
  using IterationFunction = typename FixedPointTraits<ARG>::IterationFunction;
  explicit AcceleratorBase(IterationFunction const &I)
    : M_I{I} {}; // constructor
  //! Call operator that returns the accelerated iterate
  ArgumentType virtual
  operator()(ArgumentType const &x) const
  {
    return M_I(x);
  }
  //! Puts the accelerator back to the original setting
  virtual void reset() const {};
  //! Returns the identifier of the accelerator. Useful to implement different
  //! choices run-rime
  virtual FPAcceleratorId
  getId() const
  {
    return id;
  }
  virtual ~AcceleratorBase() = default;
  static constexpr FPAcceleratorId id = None;

protected:
  //!@note I decided to aggregate the iterator function (useful if
  //! IterationFunction is a big object)
  //! and not to compose it. This is a design choice. It make the class less
  //! flexible but more efficient. Less flexible since it cannot have a default
  //! constructors, since a reference must be bound to an object at construction
  //! time. More efficient since the iterator function is not copied at each
  //! call and can be shared with the fixed-point solver without useless
  //! replications
  const IterationFunction &M_I;
  // Solution with composition
  // IteratorFunction M_I;
};
/*
An Alias for the identity accelerator (no acceleration)
*/
/**
 * @brief Alias template for a fixed-point solver without acceleration.
 *
 * This alias template defines a type `NoAcceleration` which is a specialization
 * of `AcceleratorBase` for the given fixed-point argument type `ARG`. It
 * represents a fixed-point solver that does not use any acceleration
 * techniques.
 *
 * @tparam ARG The type of the argument for the fixed-point solver.
 * possible values are `FixedPointArgumentType::VECTOR` and
 * `FixedPointArgumentType::EIGEN`
 */
template <FixedPointArgumentType ARG>
using NoAcceleration = AcceleratorBase<ARG>;

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
 * @tparam ARG. The iterator function. Note that at this level the
 * iterator function is used only as a trait to extract the type used for the
 * arguments. No other requirements are made
 *
 */
template <FixedPointArgumentType ARG>
class ASecantAccelerator : public AcceleratorBase<ARG>
{
public:
  using ArgumentType = typename AcceleratorBase<ARG>::ArgumentType;
  using IterationFunction = typename AcceleratorBase<ARG>::IterationFunction;
  using AcceleratorBase<ARG>::AcceleratorBase; // inherit constructor
  //! Call operator that returns the accelerated iterate
  ArgumentType operator()(ArgumentType const &x) const override;
  //! Put back the accelerator to initial setting
  void
  reset() const override
  {
    length = 0;
    firstTime = true;
  }
  //! Returns the identifier of the accelerator. Useful to implement different
  //! choices run-rime
  FPAcceleratorId
  getId() const override
  {
    return id;
  }
  //! The identifier is an attribute of the class. It may be tested on the type.
  static constexpr FPAcceleratorId id = ASecant;

protected:
  mutable ArgumentType deltaXOld;
  mutable ArgumentType xOld;
  mutable ArgumentType phiOld;
  mutable size_t       length{0};
  mutable ArgumentType xNew;
  mutable ArgumentType deltaXNew;
  mutable bool         firstTime = true;
};

/**
 * @class AndersonAccelerator
 * @brief Implements the Anderson acceleration method for fixed-point
 * iterations.
 *
 * This class inherits from AcceleratorBase and provides an implementation of
 * the Anderson acceleration technique, which is used to accelerate the
 * convergence of fixed-point iterations.
 *
 * @tparam ARG The type of the argument for the fixed-point iteration.
 *
 * @note The Anderson acceleration requires at least two internal iterates to
 * function properly. It can only used Eigen vectors as arguments.
 */
template <FixedPointArgumentType ARG>
  requires apsc::is_same_argument_type_v<ARG, FixedPointArgumentType::EIGEN>
class AndersonAccelerator : public AcceleratorBase<ARG>
{
public:
  using ArgumentType = typename AcceleratorBase<ARG>::ArgumentType;
  using ReturnType = ArgumentType;
  using IterationFunction = typename AcceleratorBase<ARG>::IterationFunction;
  // inherit base constructor
  using AcceleratorBase<ARG>::AcceleratorBase;
  //! @brief Constructor for the Anderson acceleration with a maximum number of
  //! stored vectors indicated
  //! @param I The iterator function
  //! @param m The maximum number of stored vectors
  explicit AndersonAccelerator(IterationFunction const &I, unsigned int m)
    : AcceleratorBase<ARG>(I), m_max(m)
  {}

  //! Set the maximum number of stored vectors
  void
  setM_max(int m)
  {
    m_max = m;
  }
  //! @brief Override the call operator
  //! @param x The new iterate
  //! @return The accelerated iterate
  ArgumentType
  operator()(ArgumentType const &x) const override
  {
    ArgumentType f_x = this->M_I(x);
#ifndef NDEBUG
    if(m_max < 2u)
      throw std::runtime_error("Anderson acceleration needs at least two "
                               "internal iterates, set m>2\n");
#endif
    // Update the history
    if(m_history.size() == this->m_max)
      { // eliminate last iterate
        m_history.pop_front();
        f_history.pop_front();
      }
    m_history.push_back(f_x);
    f_history.push_back(f_x - x);
    if(m_history.size() < 2u)
      {             // first or second time
        return f_x; // do nothing, not enough data
      }
    auto const localSize = m_history.size();
    // Form the matrices F and G
    Eigen::MatrixXd F(x.size(), localSize - 1u);
    Eigen::MatrixXd G(x.size(), localSize - 1u);
    for(std::size_t i = 0u; i < localSize - 1u; ++i)
      {
        F.col(i) = f_history[i + 1u] - f_history[i];
        G.col(i) = m_history[i + 1u] - m_history[i];
      }

    // Solve the least squares problem
    Eigen::VectorXd gamma = F.colPivHouseholderQr().solve(f_history.back());

    // Compute the new iterate

    return f_x - G * gamma;
  }

  // Override the reset method
  void
  reset() const override
  {
    m_history.clear();
    f_history.clear();
  }

  FPAcceleratorId
  getId() const override
  {
    return id;
  }

  static constexpr FPAcceleratorId id = Anderson;

private:
  unsigned int                     m_max = 10u;
  mutable std::deque<ArgumentType> m_history;
  mutable std::deque<ArgumentType> f_history;
};

template <FixedPointArgumentType ARG>
typename ASecantAccelerator<ARG>::ArgumentType
ASecantAccelerator<ARG>::operator()(ArgumentType const &x) const
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

/**
 * @brief Factory function to create accelerators based on a string identifier.
 *
 * This function takes a string identifier and returns a unique pointer to an
 * instance of the corresponding accelerator class.
 *
 * @tparam ARG The type of the argument for the fixed-point solver.
 * Possible values are `VECTOR` and `EIGEN`.
 * @param id The string identifier of the accelerator.
 * @param I The iteration function.
 * @param m Optional parameter for the Anderson accelerator indicating the
 * maximum number of stored vectors.
 * @return std::unique_ptr<AcceleratorBase<ARG>> A unique pointer to the created
 * accelerator.
 */
template <FixedPointArgumentType ARG>
std::unique_ptr<AcceleratorBase<ARG>>
createAccelerator(const std::string                                      &id,
                  typename AcceleratorBase<ARG>::IterationFunction const &I,
                  unsigned int m = 10)
{
  if(id == "NoAcceleration")
    {
      return std::make_unique<NoAcceleration<ARG>>(I);
    }
  else if(id == "ASecant")
    {
      return std::make_unique<ASecantAccelerator<ARG>>(I);
    }
  else if(id == "Anderson")
    {
      if constexpr(not apsc::is_same_argument_type_v<
                     ARG, FixedPointArgumentType::EIGEN>)
        {
          throw std::invalid_argument("Anderson acceleration requires Eigen "
                                      "vectors as arguments");
        }
      else
        {
          return std::make_unique<AndersonAccelerator<ARG>>(I, m);
        }
    }
  else
    {
      throw std::invalid_argument("Unknown accelerator type: " + id);
    }
}

} // end namespace apsc

#endif /* SRC_NONLINSYSSOLVER_ACCELERATORS_HPP_ */
