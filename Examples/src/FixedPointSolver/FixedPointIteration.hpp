/*
 * FixedPointIteration.hpp
 *
 *  Created on: Jan 25, 2020
 *      Author: forma
 */

#ifndef SRC_NONLINSYSSOLVER_FIXEDPOINTITERATION_HPP_
#define SRC_NONLINSYSSOLVER_FIXEDPOINTITERATION_HPP_
#include "Accelerators.hpp"
#include <tuple>
#include <cmath>
#include <memory>
#include <limits>
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
	double tolerance=1.e-8;
	unsigned int maxIter=100;
};

//! A class for (possibly) accelerated fixed point iterations
/*!
 * \tparam IterationFunction a system of equation that obeys the interface of NonLinearSystem contained in nonLinearSystem.hpp
 * \tparam Accelerator A template template parameter that identify an procedure to accelerate convergence.
 *
 */
  template <FixedPointArgumentType ARG, template<FixedPointArgumentType> class Accelerator=NoAccelerator>
  class FixedPointIteration
  {
  public:
    //! The type of the argument and return value
    using ArgumentType = typename FixedPointTraits<ARG>::ArgumentType;
    using AcceleratorType = Accelerator<ARG>;
    using IterationFunction = typename FixedPointTraits<ARG>::IterationFunction;
    //! Constructor that takes the iterator function and (optionally) the convergence options
    template<typename IF=IterationFunction>
    FixedPointIteration(IF&& ifun=IterationFunction(), FixedPointOptions opt=FixedPointOptions{}):
      phi{std::forward<IF>(ifun)},options{opt},accelerator(phi) {}
    //! Default constructor. IterationFunction is not set. Options are the default
    FixedPointIteration():accelerator(phi){};
    //! Getter for options
    FixedPointOptions getOptions() const {
      return options;
    }
    //! setter for options
    void setOptions(const FixedPointOptions& options) {
      this->options = options;
    }
    //! To set, or change the iteration function
    void setIterationFunction(const IterationFunction & ifun){phi=ifun;}
    //! Allow to access the accelerator
    /*!
     * In case we need to set its state.
     */
    auto & getAccelerator(){ return accelerator;}
    //! Computed the result
    /*!
     * \param x0 initial value
     * \return a tuple containing the result, the number of iteration the last increment, and a boolean that is true if convergence has been reached.
     * \note  Since C++17 you can retrieve the returned values as
     * \code
     auto [value,iter,distance,converged] = fixedpoint.compute(x0);
     * \endcode
     *
     */
    auto compute(ArgumentType const & x0)
    {
      double currentDistance{std::numeric_limits<double>::max()};
      std::size_t iter{0};
      ArgumentType current=x0;
      ArgumentType previous=x0;
      while(iter<options.maxIter && currentDistance >options.tolerance)
        {
          current = this->accelerator(previous);
          currentDistance=this->distance(current,previous);
          previous=current;
          ++iter;
#ifdef VERBOSE
          std::cout<<"Iteration:"<<iter<<" Values:(";
          for (auto j=0;j<current.size();++j)
            std::cout<<current[j]<<", ";
          std::cout<<std::endl;
#endif
        }
      this->accelerator.reset();
      return std::make_tuple(current,iter,currentDistance,(iter<options.maxIter));
    }
  private:
    IterationFunction phi;
    FixedPointOptions options;
    AcceleratorType accelerator;
    double distance(ArgumentType const & current, ArgumentType const & previous)
    {
      double res{0.0};
      for (std::size_t i=0; i<current.size();++i)
        res+=(previous[i]-current[i])*(previous[i]-current[i]);
      return std::sqrt(res);
    }
  };

}




#endif /* SRC_NONLINSYSSOLVER_FIXEDPOINTITERATION_HPP_ */
