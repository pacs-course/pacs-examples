/*!
 * @file preconditionerFactory.hpp
 * @brief Small runtime factory used by the example driver to instantiate the
 *        selected preconditioner.
 */

#ifndef EXAMPLES_SRC_LINEARALGEBRA_IML_EIGEN_SADDLEPOINTSOLVER_PRECONDITIONERFACTORY_HPP_
#define EXAMPLES_SRC_LINEARALGEBRA_IML_EIGEN_SADDLEPOINTSOLVER_PRECONDITIONERFACTORY_HPP_
#include "BasicType.hpp"
#include "preconditioner.hpp"
#include <functional>
#include <memory>
#include <unordered_map>
namespace FVCode3D
{
using PrecondBuilder = std::function<std::unique_ptr<preconditioner>()>;
using PreconditionerFactory = std::unordered_map<PrecondSwitch, PrecondBuilder>;
/*!
 * Build the map from `PrecondSwitch` values to nullary constructors.
 *
 * The main program uses this factory after parsing `data.pot`, so no
 * preconditioner type needs to be mentioned explicitly in the solver loop.
 *
 * @return Factory map keyed by `PrecondSwitch`.
 */
PreconditionerFactory
make_PreconditionerFactory()
{
  using namespace FVCode3D;
  PreconditionerFactory factory;
  factory[Identity] = []() {
    return std::make_unique<identity_preconditioner>();
  };
  factory[Diagonal] = []() {
    return std::make_unique<diagonal_preconditioner>();
  };
  factory[BlockDiagonal] = []() {
    return std::make_unique<BlockDiagonal_preconditioner>();
  };
  factory[BlockTriangular] = []() {
    return std::make_unique<BlockTriangular_preconditioner>();
  };
  factory[ILU] = []() { return std::make_unique<ILU_preconditioner>(); };
  factory[HSS] = []() { return std::make_unique<HSS_preconditioner>(); };
  factory[DoubleSaddlePoint] = []() {
    return std::make_unique<DoubleSaddlePoint_preconditioner>();
  };
  factory[DoubleSaddlePointSym] = []() {
    return std::make_unique<DoubleSaddlePointSym_preconditioner>();
  };
  return factory;
}
} // namespace FVCode3D

#endif /* EXAMPLES_SRC_LINEARALGEBRA_IML_EIGEN_SADDLEPOINTSOLVER_PRECONDITIONERFACTORY_HPP_ \
        */
