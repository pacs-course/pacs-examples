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

#ifdef PARALLELSTD
#include <execution> //if you use parallel algorithm
#endif

#include <type_traits>
#include <utility>
namespace apsc
{
/*!
 * @brief An example of a class that stores a non linear system.
 *
 * The system \f$ R^n\rightarrow R^m\f$ is stored as a vector of functions
 * \f$ R^n\rightarrow R\f$. You may call the single function or the whole system
 * In fact \f$R\f$ is a generic type representing a field (it may be a complex).
 *
 * The types used in this class are taken from a trait templatised on R. You can
 * change the trait if you want to change the types used in this class (for
 * instance if you want to use Eigen vectors).
 *
 * @tparam T The type of the arguments and return of the functions
 * @tparam Traits The trait defining all the basic type used in this class
 */
template <typename R = double, template <typename> typename Traits =
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
    //@todo use concepts instead of static_assert
    static_assert(std::is_convertible_v<scalarfunction,
                                        typename Traits<R>::ScalarFunctionType>,
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
   * @brief change the i-th function
   *
   * @tparam scalarfunction A function wrapper type.
   * @param i The index
   * @return A function \f$ R^n\rightarrow R\f$
   */
  template <typename scalarfunction>
  void
  updateFunction(scalarfunction &&f, unsigned int i)
  {
    static_assert(std::is_convertible_v<scalarfunction,
                                        typename Traits<R>::ScalarFunctionType>,
                  "Cannot add a function with wrong signature!");
    system_[i] = std::forward<scalarfunction>(f);
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
    /* //standard for loop
     for (auto const & fun: system_)
      {
        res.emplace_back(fun(x));
      }
      */
    // Using parallel algorithms
    // you have to include <execution> and link with -ltbb
#ifdef PARALLELSTD
    std::for_each(std::execution::par, system_.begin(), system_.end(),
                  [&x, &res_](auto const &fun) { res_.emplace_back(fun(x)); });
#else
    // to use openMP you have to compile with -fopenmp
#pragma omp parallel for
    for(std::size_t i = 0; i < system_.size(); ++i)
      {
        res_[i] = system_[i](x);
      }
#endif
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
  FunctionContainerType system_;
};

} // namespace apsc

#endif /* EXAMPLES_SRC_NONLYNSYS_NONLINSYS_HPP_ */
