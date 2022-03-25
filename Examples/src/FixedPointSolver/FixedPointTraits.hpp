/*
 * FixedPointTraits.hpp
 *
 *  Created on: Feb 25, 2020
 *      Author: forma
 */

#ifndef NONLINSYSSOLVER_FIXEDPOINTTRAITS_HPP_
#define NONLINSYSSOLVER_FIXEDPOINTTRAITS_HPP_
#include <Eigen/Dense>
#include <vector>
namespace apsc
{
enum class FixedPointArgumentType
{
  VECTOR,
  EIGEN
};

template <FixedPointArgumentType ARG>
struct FixedPointTraits
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

} // namespace apsc

#endif /* NONLINSYSSOLVER_FIXEDPOINTTRAITS_HPP_ */
