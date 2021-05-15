/*
 * preconditionerWrapper.hpp
 *
 *  Created on: May 12, 2020
 *      Author: forma
 */

#ifndef EXAMPLES_SRC_LINEARALGEBRA_IML_EIGEN_SADDLEPOINTSOLVER_PRECONDITIONERFACTORY_HPP_
#define EXAMPLES_SRC_LINEARALGEBRA_IML_EIGEN_SADDLEPOINTSOLVER_PRECONDITIONERFACTORY_HPP_
#include "preconditioner.hpp"
#include  "BasicType.hpp"
#include <unordered_map>
#include <functional>
#include <memory>
namespace FVCode3D
{
 using PrecondBuilder=std::function<std::unique_ptr<preconditioner>()>;
 using PreconditionerFactory=std::unordered_map<PrecondSwitch,PrecondBuilder>;
 /*!
  * A simple factory for the preconditioners
  * @return The factory (a std::map)
  */
 PreconditionerFactory make_PreconditionerFactory()
 {
   using namespace FVCode3D;
   PreconditionerFactory factory;
   factory[Identity]=[](){return std::make_unique<identity_preconditioner>();};
   factory[Diagonal]=[](){return std::make_unique<diagonal_preconditioner>();};
   factory[BlockDiagonal]=[](){return std::make_unique<BlockDiagonal_preconditioner>();};
   factory[BlockTriangular]=[](){return std::make_unique<BlockTriangular_preconditioner>();};
   factory[ILU]=[](){return std::make_unique<ILU_preconditioner>();};
   factory[HSS]=[](){return std::make_unique<HSS_preconditioner>();};
   factory[DoubleSaddlePoint]=[](){return std::make_unique<DoubleSaddlePoint_preconditioner>();};
   factory[DoubleSaddlePointSym]=[](){return std::make_unique<DoubleSaddlePointSym_preconditioner>();};
return factory;
}
}




#endif /* EXAMPLES_SRC_LINEARALGEBRA_IML_EIGEN_SADDLEPOINTSOLVER_PRECONDITIONERFACTORY_HPP_ */
