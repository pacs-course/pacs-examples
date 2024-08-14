/*
 * VectorInterface.hpp
 *
 *
 *  Created on: Jan 27, 2021
 *      Author: forma
 */

#ifndef EXAMPLES_SRC_FIXEDPOINTSOLVER_VECTORINTERFACE_HPP_
#define EXAMPLES_SRC_FIXEDPOINTSOLVER_VECTORINTERFACE_HPP_
#include <Eigen/Core>
#include <numeric>
#include <vector>
// This nested namespace is meant to hide the functions defined here. You should
// use the fullly qualified name or using namespace apsc::internals if you want
// to use them
//
// This file provides adapters in the form of function to enable to use
// std::vector and Eigen vectors in a seamless way. This way we have a common interface
// to several utilities between std::vector and Eigen::VectorXd

namespace apsc::internals
{
using Vector = std::vector<double>;

/*!
 * Eigen implements the dot product as a method of the class of vectors (which
 * in Eigen are matrices with one dimension equal to 1). Clearly
 * std:::vector<double> has not a method called dot. So I create a function that
 * exploits the dot() method but provides an interfaace usable also for
 * std::vectors. To do things properly with the Eigen library it is better to
 * pass the Eigen vectors as references to MatrixBase<Derived>, where Derived is
 * a template parameter. The reason (well explained in the Eigen documentation)
 * is due to the fact that the Eigen uses expression templates. In this specific
 * case it is not strictly necessary, but is you do things following the Eigen
 * rule it is safer. Moreover, I do not know what dot() returns. Certainly
 * something that is convertible to a floating point, but really I do not care.
 * I use decltype(auto) (not simply auto) to be sure to return a value with
 * exactly the same type returned by dot();
 *
 * I wanted to do something more general than necessary. In this form the
 * parameters a and b may be two Eigen vectors of different type, for instance
 * one can store floats and the other doubles. This generality is not necesssary
 * for the FixedPointSolver code, but since it does'nt cost much I prefer to
 * adopt it.
 *
 * @tparam Derived Defines the type of the first Eigen vector (matrix)
 * @tparam OtherDerived Defines the type of the second Eigen vector (matrix)
 * @param a The fist vector
 * @param b The second vector
 * @return The dot product
 */
template <typename Derived, typename OtherDerived>
decltype(auto)
dot(const Eigen::MatrixBase<Derived> &     a,
    const Eigen::MatrixBase<OtherDerived> &b)
{
  return a.dot(b);
}
/*!
 * The overload for standard vectors. Also here for generality I accept vectors
 * storing different types ( I couls have decided to make the overload only for
 * std::vector<double>) But I always return a double (to make things simpler, I
 * do not need more complexity)
 * @tparam The type stored in the vector (a floating point)
 * @param a the first standard vector
 * @param b the second standard vector
 * @return The dot product
 */
template <class T>
T
dot(std::vector<T> const &a, std::vector<T> const &b)
{
  // I use the standard algorithm that does it
  // NOTE: This algorithm can be made parallel by adding an execution policy!!
  return std::transform_reduce(a.begin(), a.end(), b.begin(), T{0});
}

/*!
 * @brief Squared norm of an Eigen vector
 * @details Eigen matrices (and thus also vectors) have a method sqaredNorm that returns
 * the square of the norm. I need to have an adapter that provides an interface
 * usable also with standard vectors. The considerations made for dot() apply
 * also here.
 * @tparam Derived Defines the type of the vector
 * @param a The vector
 * @return The square of the 2-norm of the vector
 */
template <typename Derived>
decltype(auto)
squaredNorm(const Eigen::MatrixBase<Derived> &a)
{
  return a.squaredNorm();
}
/*!
 * The definition of squaredNorm for vector
 * @tparam T The type stored in the vector
 * @param a the vector
 * @return the squared norm
 */
template <typename T>
T
squaredNorm(std::vector<T> const &a)
{
  return dot(
    a, a); // Avoid code duplication! You have already defined the dot product!
}

/*!
 * Addition for vectors
 * @tparam T The type contained in the vector
 * @param a The first vector
 * @param b The second vector
 * @return the addition
 * @pre The two vectors must have the same size
 */
template <class T>
std::vector<T>
operator+(std::vector<T> const &a, std::vector<T> const &b)
{
  std::vector<T> res(a.size, 0); // a vector of zeros
  // I use std::transform because, by adding an execution policy I can make it
  // parallel! A note: the non parallel version can be made more efficient...
  // but then I cannot use std::transform
  std::transform(a.begin(), a.end(), b.begin(), res.begin(),
                 std::plus<T>());
  /* a non parallel version with a simple loop, made more efficient (less
  operations) std::vector<T> res=a; for (std::size_t i=0u;i<a.size();++i)
    res[i]+=b[i]; // add-assign!
    */
  return res;
}

/*!
  * Subtraction for vectors
  * @tparam T The type contained in the vector
  * @param a The first vector
  * @param b The second vector
  * @return the subtraction
  * @pre The two vectors must have the same size
*/
template <typename T>
std::vector<T>
operator-(std::vector<T> const &a, std::vector<T> const &b)
{
  std::vector<T> res(a.size(), 0);
  // I use std::transform because, by adding an execution policy I can make it
  // parallel! the seme considerations for the + version apply here.
  std::transform(a.begin(), a.end(), b.begin(), res.begin(),
                 std::minus<T>());
  return res;
}
/*!
 * Pre-multiplication by a scalar. The scalar must of of the same type contained
 * in the vector
 * @note Beware! implicit conversion is NOT activated on template arguments. The
 * scalar must be of the same type of the vector's elements.
 * @tparam T The type contained in the vector
 * @param a A scalar
 * @param b A vector
 * @return a*b
 */
template <typename T>
std::vector<T>
operator*(T const &a, std::vector<T> const &b)
{
  std::vector<T> res = b;
  // I use std::transform because, by adding an execution policy I can make it
  // parallel!
  std::transform(res.begin(), res.end(), res.begin(),
                 [&a](T const &y) { return a * y; });
  /* version with a simple loop (it works fine as well)
  for (auto & x:res) x*=a;
  */
  return res;
}

/*!
 * Post-multiplication by a scalar
 * Since I have a*b I want to have also b*a...
 * @tparam T The type contained in the vector
 * @param a A scalar
 * @param b A vector
 * @return b*a
 */
template <typename T>
std::vector<T>
operator*(std::vector<T> const &b, T const &a)
{
  return a * b; // no code duplication!
}

/*!
 * Often we need the squared distance of two vectors. I wrote an utility for the
 * purpose. Here the version for Eigen.
 * @tparam Derived The type of the first vector
 * @tparam OtherDerived The type of the second vector
 * @param a the first vector
 * @param b the second vector
 * @return ||a-b||^2
 * @note The return type is the same as the one returned by squaredNorm()
 * @pre The two vectors must have the same size
 */
template <typename Derived, typename OtherDerived>
decltype(auto)
squaredDistance(const Eigen::MatrixBase<Derived> &     a,
                Eigen::MatrixBase<OtherDerived> const &b)
{
  return (a - b).squaredNorm();
}
/*!
  * The version for standard vectors
  * @tparam T The type contained in the vector
  * @param a The first vector
  * @param b The second vector
  * @return ||a-b||^2
  * @note The return type is the same as the one returned by squaredNorm()
  * @pre The two vectors must have the same size
  */
template <typename T>
T
squaredDistance(std::vector<T> const &a, std::vector<T> const &b)
{
  return squaredNorm(a - b);
}

} // end namespace apsc::internals

#endif /* EXAMPLES_SRC_FIXEDPOINTSOLVER_VECTORINTERFACE_HPP_ */
