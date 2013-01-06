/*
 * bc_factory1.hpp
 *
 *  Created on: Nov 28, 2008
 *      Author: forma
 */
#ifndef BC_CONTAINER_HPP_
#define BC_CONTAINER_HPP_
#include <vector>
#include "bound_cond.hpp"
namespace FEM{
  //! A simple bc container
  typename std::vector<BCBase> BCContainer;

  //! Extracts all boundary conditions of a given type
  std::vector<BCContainer::const_iterator> extractBCWithType(BCContainer const & bcCOntainer,BCType const & type);

  //! Extracts all boundary conditions of a given name
  std::vector<BCContainer::const_iterator> extractBCWithName(BCContainer const & bcCOntainer,BCName const & name);
  
  void addToBCContainer(BCName const & name, BCType const & type,
			BCFun const & f, BCContainer & bcContainer);
}
  
#endif /* BC_CONTAINER_HPP_ */
