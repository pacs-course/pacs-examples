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
#include <cstddef>
#include <concepts>
#include <ranges>

#ifdef PARALLELSTD
#include <execution> //if you use parallel algorithm
#endif

#include <stdexcept>
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
  explicit NonLinSys(std::size_t numEqs = 0u) { this->reserve(numEqs); }
  /*!
   * If you know the size of the system better reserve the space
   * @param numEqs  Number of equations (0= no reserve)
   */
  void
  reserve(std::size_t numEqs)
  {
    system_.reserve(numEqs);
  }

  /*!
   * Adds a function to the system
   * @tparam scalarfunction A function wrapper type.
   * @param f A function \f$ R^n\rightarrow R\f$ to be added
   * @note Note the use of forwarding
   */
  template <typename scalarfunction>
    requires std::convertible_to<scalarfunction,
                                 typename Traits<R>::ScalarFunctionType>
  void
  addToSystem(scalarfunction &&f)
  {
    // before concepts you could still check with static_assert
    // you can still do it, nothing wrong with it (and works also with no C++20
    // compliant compilers)
    // static_assert(std::is_convertible_v<scalarfunction,
    //                                    typename
    //                                    Traits<R>::ScalarFunctionType>,
    //              "Cannot add a function with wrong signature!");
    // Note the use of std::forward, to enjoy the benefits of move semantics
    system_.emplace_back(std::forward<scalarfunction>(f));
  }
  /*!
   * Get the i-th function
   * @param i The index
   * @return A function \f$ R^n\rightarrow R\f$
   * @note Returned as a const reference to allow s.getFunction(2)(3.1415) with
   * no useless copies
   */
  [[nodiscard]] ScalarFunctionType const &
  getFunction(std::size_t i) const
  {
#ifndef NDEBUG 
    if(i >= system_.size()) // check bounds only if debugging, otherwise just let the vector throw the exception
      {
        throw std::out_of_range("Function index out of range");
      }
#endif
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
    requires std::convertible_to<scalarfunction,
                                 typename Traits<R>::ScalarFunctionType>
  void
  updateFunction(scalarfunction &&f, std::size_t i)
  {
#ifndef NDEBUG
    if(i >= system_.size()) // bounds checks only if debugging, otherwise just let the vector throw the exception
      {
        throw std::out_of_range("Function index out of range");
      }
#endif
    system_[i] = std::forward<scalarfunction>(f);
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
    // Note that to get the index I need to use some features of pointer
    // arithmetics
#ifdef PARALLELSTD
    auto indices = std::views::iota(0u, system_.size()) | std::views::common;
    std::for_each(
      std::execution::par, indices.begin(), indices.end(),
      [&x, &res_, this](auto const &i) { res_[i] = system_[i](x); });
#else
    // Use OpenMP only when the compiler enables it, otherwise fallback to a
    // standard serial loop.
#ifdef _OPENMP
#pragma omp parallel for
#endif
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
  [[nodiscard]] std::size_t
  numEqs() const
  {
    return system_.size();
  }

  // I want the system of equation satisfy the concept of a sized range
  // so I need to implement the following methods
  // I can use the default implementation for the iterators
  // since I have a vector
  // I can use the default implementation for the size
  // since I have a vector
  // I can use the default implementation for the begin and end
  // since FunctionContainer is a vector
  //*
  /*!
   * @return An iterator to the beginning of the system
   */
  [[nodiscard]] auto
  begin() const
  {
    return system_.cbegin();
  }
  /*!
   * @return An iterator to the end of the system
   */
  [[nodiscard]] auto
  end() const
  {
    return system_.cend();
  }
  /*!
   * @return The size of the system
   */
  [[nodiscard]] std::size_t
  size() const
  {
    return system_.size();
  }

protected:
  FunctionContainerType system_;
};

} // namespace apsc

#endif /* EXAMPLES_SRC_NONLYNSYS_NONLINSYS_HPP_ */
