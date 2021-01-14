/*
 * bc_factory1.hpp
 *
 *  Created on: Nov 28, 2008
 *      Author: forma
 */
#ifndef BC_CONTAINER_HPP_
#define BC_CONTAINER_HPP_
#include <vector>
#include <set>
#include "bound_cond.hpp"
namespace apsc::FEM{
  //! A simple bc container
  using BCContainer=std::vector<BCBase>;
  
  //! Extracts all boundary conditions of a given type
  std::vector<BCBase> extractBCWithType(BCContainer const & bcCOntainer,BCType const & type);
  
  //! Extracts all boundary conditions of a given name
  std::vector<BCBase> extractBCWithName(BCContainer const & bcCOntainer,BCName const & name);
  
}
  
#endif /* BC_CONTAINER_HPP_ */
