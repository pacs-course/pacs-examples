#ifndef JACOBIANFACTORY_HPP
#define JACOBIANFACTORY_HPP

#include "Jacobian.hpp"

#include <memory>

/// Enumerator for different Jacobian types.
///
/// A simple enumerator is easily convertible to an int.
/// The numbering is explicitely indicated, only to facilitate the
/// identification of the integer corresponding to the enum (if it should be
/// parsed from a file, for instance).
enum class JacobianType : unsigned int
{
  Discrete = 0,
  Full     = 1
};

/// A simple factory that returns a JacobianBase polymorphic object
/// wrapped in a unique_ptr.
///
/// @tparam kind The type of Jacobian class desired.
///
/// @param args Arguments to be forwarded to the constructor.
template <ProblemType Type, JacobianType JacType, class... Args>
std::unique_ptr<JacobianBase<Type>>
make_jacobian(Args &&... args)
{
  static_assert(JacType == JacobianType::Discrete ||
                  JacType == JacobianType::Full,
                "Error in JacobianType: wrong type specified.");

  if constexpr (JacType == JacobianType::Discrete)
    return std::make_unique<DiscreteJacobian<Type>>(
      std::forward<Args>(args)...);
  else // if constexpr (JacType == JacobianType::Full)
    return std::make_unique<FullJacobian<Type>>(std::forward<Args>(args)...);
}

#endif /* JACOBIANFACTORY_HPP */
