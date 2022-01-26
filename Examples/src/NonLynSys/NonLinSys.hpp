/*
 * NonLynSys.hpp
 *
 *  Created on: Feb 12, 2021
 *      Author: forma
 */

#ifndef EXAMPLES_SRC_NONLYNSYS_NONLINSYS_HPP_
#define EXAMPLES_SRC_NONLYNSYS_NONLINSYS_HPP_
#include "NonLinSysTraits.hpp"
#include <algorithm>
#include <execution>
#include <utility>
#include <type_traits>
namespace apsc
{
/*!
 * An example of a class that stores a non linear system.
 *
 * The system \f$ R^n\rightarrow R^m\f$ is stored as a vector of functions
 * \f$ R^n\rightarrow R\f$. You may call the single function or the whole system
 * In fact \f$R\f$ is a generic type (it may be a complex).
 *
 * The types used in this class are taken from a trait templatised on R. You can
 * change the trait if you fant to change the basic tyes used in this class (for
 * instance if you want to use Eigen vectors).
 *
 * @tparam T The type of the arguments and return of the functions
 * @tparam Traits The trait defining all the basic type used in this class
 */
template <typename R, template <typename> typename Traits =
                        apsc::NonLinSysTraits::VectorTraits>
class NonLinSys
{
public:
  using ScalarType = typename Traits<R>::ScalarType;
  using ScalarFunctionType = typename Traits<R>::ScalarFunctionType;
  using ArgumentType = typename Traits<R>::ArgumentType;
  using ResultType = typename Traits<R>::ResultType;
  using FunctionContainerType = typename Traits<R>::FunctionContainerType;

  /*!
   * If you know the size of the system better reserve the space
   * @param numEqs  Number of equations (0= no reserve)
   */
  NonLinSys(std::size_t numEqs = 0u) { this->reserve(numEqs); }
  /*!
   * If you know the size of the system better reserve the space
   * @param numEqs  Number of equations (0= no reserve)
   */
  void
  reserve(std::size_t numEqs)
  {
    if(numEqs > 0u)
      {
        system_.reserve(numEqs);
      }
  }

  /*!
   * Adds a function to the system
   * @tparam scalarfunction A function wrapper type.
   * @param f A function \f$ R^n\rightarrow R\f$ to be added
   * @note Note the use of forwarding
   */
  template <typename scalarfunction>
  void
  addToSystem(scalarfunction &&f)
  {
    static_assert(std::is_convertible_v<scalarfunction,typename Traits<R>::ScalarFunctionType>,
                  "Cannot add a function with wrong signature!");
    system_.emplace_back(std::forward<scalarfunction>(f));
  }
  /*!
   * Get the i-th function
   * @param i The index
   * @return A function \f$ R^n\rightarrow R\f$
   */
  ScalarFunctionType
  getFunction(unsigned int i) const
  {
    return system_[i];
  }
  /*!
   * Get the i-th function
   * @param i The index
   * @return A function \f$ R^n\rightarrow R\f$
   */
  ScalarFunctionType &
  getFunction(unsigned int i)
  {
    return system_[i];
  }
  /*!
   * The call operator for a single function
   * @param i The index of the function
   * @param x The argument
   * @return The computed scalar value
   */
  ScalarType
  operator()(std::size_t i, const ArgumentType &x) const
  {
    return system_[i](x);
  }

  /*!
   * The call operator for the system
   * @param x The argument
   * @return The computed vector value
   */
  ResultType
  operator()(const ArgumentType &x) const
  {
    // Make sure that the elements are creted
    // otherwise I cannot use the parallel algorithms because
    // of data race when increasing size
    ResultType res_;
    res_.resize(system_.size());
    /*
     for (auto const & fun: system_)
      {
        res.emplace_back(fun(x));
      }
      */
    // Using parallel algorithms
#pragma omp parallel for
    for(std::size_t i = 0; i < system_.size(); ++i)
      {
        res_[i] = system_[i](x);
      }
    return res_;
  }
  /*!
   * @return Current number of equations
   */
  std::size_t
  numEqs() const
  {
    return system_.size();
  }

protected:
  FunctionContainerType         system_;
};

} // namespace apsc

#endif /* EXAMPLES_SRC_NONLYNSYS_NONLINSYS_HPP_ */
