#ifndef JACOBIANFACTORY_HPP
#define JACOBIANFACTORY_HPP

#include "Jacobian.hpp"

#include <iostream>
#include <memory>

/// Enumerator for different Jacobian types.
///
/// I use a simple enumerator because it is convertible to an int
/// I explicitely indicate the numbering, only to facilitate the
/// identification of the integer corresponding to thee enum (if I
/// want to read it from a file, for instance)
///
enum class JacobianType
{
  Discrete,
  Full
};

/// A simple factory that returns a JacobianBase polymorphic object
/// wrapped in a unique_prt.
///
/// /tparam kind The kind of Jacobian type class you want.
///
/// /param args Optional arguments to be forwarded to the
/// constructor.
///
template <JacobianType Type, typename... Args>
std::unique_ptr<JacobianBase>
make_Jacobian(Args &&... args)
{
  static_assert(
    Type == JacobianType::Discrete || Type == JacobianType::Full,
    "Error in JacobianType: you must specify a valid type.");

  if constexpr (Type == JacobianType::Discrete)
    {
      return std::make_unique<DiscreteJacobian>(
        std::forward<Args>(args)...);
    }
  else // if constexpr (Type == JacobianType::Full)
    {
      return std::make_unique<FullJacobian>(
        std::forward<Args>(args)...);
    }
}

#endif /* JACOBIANFACTORY_HPP */
