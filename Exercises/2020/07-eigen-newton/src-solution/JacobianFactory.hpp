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
enum class JacobianKind
{
  Discrete,
  Full
};

/// A simple factory that returns a JacobianBase polymorphic object
/// wrapped in a unique_prt
///
/// /param kind The kind of Jacobian type class you want
/// /param args Optional arguments to be forwarded to the
/// constructor
///
template <JacobianKind Kind, typename... Args>
std::unique_ptr<JacobianBase>
make_Jacobian(Args &&... args)
{
  if constexpr (Kind == JacobianKind::Discrete)
    {
      return std::make_unique<DiscreteJacobian>(
        std::forward<Args>(args)...);
    }
  else if constexpr (Kind == JacobianKind::Full)
    {
      return std::make_unique<FullJacobian>(
        std::forward<Args>(args)...);
    }
  else
    {
      throw std::runtime_error(
        "Error in JacobianKind: you must specify a valid type");
    }
}

#endif /* JACOBIANFACTORY_HPP */
