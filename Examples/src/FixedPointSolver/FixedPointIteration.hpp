/*
 * FixedPointIteration.hpp
 *
 *  Created on: Jan 25, 2020
 *      Author: forma
 */

#ifndef SRC_NONLINSYSSOLVER_FIXEDPOINTITERATION_HPP_
#define SRC_NONLINSYSSOLVER_FIXEDPOINTITERATION_HPP_
#include "Accelerators.hpp"
#include "VectorInterface.hpp"
#include <cmath>
#include <limits>
#include <memory>
#include <tuple>
#ifdef VERBOSE
// If I want verbosity I need to compile with -DVERBOSE
// use make CPPFLAGS+=-DVERBOSE
#include <iostream>
#endif
namespace apsc
{
//! Options for fixed point iteration
struct FixedPointOptions
{
  double       tolerance = 1.e-8;
  unsigned int maxIter = 100;
};

//! A class for (possibly) accelerated fixed point iterations
/*!
 * @param ARG the type used for the iterator function arguments (EIGEN or
 * VECTOR)
 *
 */
template <FixedPointArgumentType ARG = FixedPointArgumentType::VECTOR>
class FixedPointIteration
{
public:
  //! The type of the argument and return value
  using ArgumentType = typename FixedPointTraits<ARG>::ArgumentType;
  using AcceleratorType = std::unique_ptr<AcceleratorBase<ARG>>;
  using IterationFunction = typename FixedPointTraits<ARG>::IterationFunction;

  /**
   * @brief Constructs a FixedPointIteration object.
   *
   * @tparam IF Type of the iteration function.
   * @param ifun The iteration function.
   * @param opt Options for the fixed-point iteration, defaults to
   * FixedPointOptions.
   * @param AccelarationType Type of acceleration to use, defaults to
   * "NoAcceleration". possible other values are "ASecant" and "Anderson".
   * @param m Parameter for the accelerator (used only for Anderson), defaults
   * to 10.
   * @note I am using the trick of forward reference to avoid copies of the
   * iteration function in case the argument can be moved. This is a C++11
   * feature.
   */
  template <typename IF>
  explicit FixedPointIteration(IF              &&ifun,
                               FixedPointOptions opt = FixedPointOptions{},
                               std::string  AccelarationType = "NoAcceleration",
                               unsigned int m = 10)
    : phi{std::forward<IF>(ifun)}, options{opt},
      accelerator(apsc::createAccelerator<ARG>(AccelarationType, phi, m))
  {}
  //! This class is not default constructible. Here I state it explicitely
  FixedPointIteration() = delete;
  //! This class is not copy constructible. Here I state it explicitely
  /*!
   * @note This is a design choice. The accelerator is a unique pointer to a
   * polymprphic object that has not been designed to be copied. To make the
   * class copyable we need a strong revision of the design.
   */
  FixedPointIteration(FixedPointIteration const &) = delete;
  //! This class is not copy assigneable. Here I state it explicitly

  FixedPointIteration &operator=(FixedPointIteration const &) = delete;
  //! This class is move constructible. Here I state it explicitly
  FixedPointIteration(FixedPointIteration &&) = default;
  //! This class is move assignable. Here I state it explicitly
  FixedPointIteration &operator=(FixedPointIteration &&) = default;

  //! Getter for options
  FixedPointOptions
  getOptions() const
  {
    return options;
  }
  /*
  change acceleration method
  */
  void
  setAcceleration(std::string AccelarationType, unsigned int m = 10)
  {
    accelerator = apsc::createAccelerator<ARG>(AccelarationType, phi, m);
  }
  //! setter for options
  void
  setOptions(const FixedPointOptions &options)
  {
    this->options = options;
  }
  //! To set, or change the iteration function
  template <typename IF = IterationFunction>
  void
  setIterationFunction(IterationFunction &&ifun)
  {
    phi = std::forward<IF>(ifun);
  }
  //! Allow to access the accelerator
  /*!
   * In case we need to change its state.
   */
  auto &
  getAccelerator()
  {
    return accelerator;
  }
  //! Computed the result
  /*!
   * \param x0 initial value
   * \return a tuple containing the result, the number of iteration the last
   increment, and a boolean that is true if convergence has been reached.
   * \note  Since C++17 you can retrieve the returned values as
   * \code
   auto [value,iter,distance,converged] = fixedpoint.compute(x0);
   * \endcode
   *
   */
  auto
  compute(ArgumentType const &x0) const
  {
    double       currentDistance{std::numeric_limits<double>::max()};
    std::size_t  iter{0};
    ArgumentType current = x0;
    ArgumentType previous = x0;
    while(iter < options.maxIter && currentDistance > options.tolerance)
      {
        current = std::invoke(*accelerator, previous);
        currentDistance =
          std::sqrt(internals::squaredDistance(current, previous));
        previous = current;
        ++iter;
#ifdef VERBOSE
        std::cout << "Iteration:" << iter << " Values:(";
        for(auto j = 0; j < current.size(); ++j)
          std::cout << current[j] << ", ";
        std::cout << std::endl;
#endif
      }
    accelerator->reset();
    return std::make_tuple(current, iter, currentDistance,
                           (iter < options.maxIter));
  }

private:
  IterationFunction phi;
  FixedPointOptions options;
  AcceleratorType   accelerator;
};

} // namespace apsc

#endif /* SRC_NONLINSYSSOLVER_FIXEDPOINTITERATION_HPP_ */
