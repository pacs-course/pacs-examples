#ifndef _SETBC_HPP
#define _SETBC_HPP
#include "bcContainer.hpp"
/*!  Creates the BC for the problem This simple structure has only the
  role set up through the constructor the bc of the problem, which are
  then mede available through a container */
namespace FEM{
  struct BcGet
  {
    FEM::BCContainer bConditions;
    BcGet();
  };
  //! 
  extern BCContainer & boundaryConditions;
}
#endif
