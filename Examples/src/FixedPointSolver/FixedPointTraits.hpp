/*
 * FixedPointTraits.hpp
 *
 *  Created on: Feb 25, 2020
 *      Author: forma
 */

#ifndef NONLINSYSSOLVER_FIXEDPOINTTRAITS_HPP_
#define NONLINSYSSOLVER_FIXEDPOINTTRAITS_HPP_
#include <Eigen/Dense>
#include <type_traits>
#include <vector>
/**
 * @file FixedPointTraits.hpp
 * @brief This file contains the definition of traits and utilities for
 * fixed-point iteration methods.
 *
 * The file defines the following:
 * - An enumeration `FixedPointArgumentType` to specify the type of argument
 * used in fixed-point iterations.
 * - A primary template `FixedPointTraits` which is specialized for different
 * `FixedPointArgumentType` values.
 * - Specializations of `FixedPointTraits` for `FixedPointArgumentType::VECTOR`
 * and `FixedPointArgumentType::EIGEN`.
 * - A custom trait `is_same_argument_type` to check if two
 * `FixedPointArgumentType` values are the same.
 * - A constexpr variable template `is_same_argument_type_v` for ease of use of
 * the `is_same_argument_type` trait.
 *
 * The `FixedPointTraits` template provides the following type aliases for each
 * specialization:
 * - `ArgumentType`: The type of the argument used in the iteration function.
 * - `ReturnType`: The type returned by the iteration function.
 * - `IterationFunction`: A `std::function` type representing the iteration
 * function.
 *
 * The `is_same_argument_type` trait and `is_same_argument_type_v` variable
 * template are used to perform compile-time checks on the equality of
 * `FixedPointArgumentType` values.
 */
namespace apsc
{
/*!
  * @brief An enumeration to specify the type of argument used in fixed-point
  iterations. We can have two possible way of defining the argument for the
  fixed-point iteration:
  - `VECTOR`: The argument is a `std::vector<double>`.
  - `EIGEN`: The argument is an `Eigen::Matrix<double, Eigen::Dynamic, 1>`.
*/
enum class FixedPointArgumentType
{
  VECTOR,
  EIGEN
};

template <FixedPointArgumentType ARG> struct FixedPointTraits
{};

template <> struct FixedPointTraits<FixedPointArgumentType::VECTOR>
{
  using ArgumentType = std::vector<double>;
  using ReturnType = ArgumentType;
  using IterationFunction = std::function<ReturnType(ArgumentType const &)>;
};

template <> struct FixedPointTraits<FixedPointArgumentType::EIGEN>
{
  using ArgumentType = Eigen::Matrix<double, Eigen::Dynamic, 1>;
  using ReturnType = ArgumentType;
  using IterationFunction = std::function<ReturnType(ArgumentType const &)>;
};

/*!
@brief A type trait to check if two `FixedPointArgumentType` values are the
same.
@tparam Value1 The first `FixedPointArgumentType` value.
@tparam Value2 The second `FixedPointArgumentType` value.
@details It is an example of template metaprogramming that extends the
`std::is_same` trait to work with `FixedPointArgumentType` values. Unfortunately
(so far) C++ deas not provide a specialization of `std::is_same` for
enumerators.
*/
template <FixedPointArgumentType Value1, FixedPointArgumentType Value2>
struct is_same_argument_type : std::false_type
{};

/*
@brief This specialization is activated if the two `FixedPointArgumentType`
values are the same.
*/
template <FixedPointArgumentType Value>
struct is_same_argument_type<Value, Value> : std::true_type
{};

/*!
@brief A helper variable template to check if two `FixedPointArgumentType`
values are the same.
*/
template <FixedPointArgumentType Value1, FixedPointArgumentType Value2>
constexpr bool is_same_argument_type_v =
  is_same_argument_type<Value1, Value2>::value;

} // namespace apsc

#endif /* NONLINSYSSOLVER_FIXEDPOINTTRAITS_HPP_ */
