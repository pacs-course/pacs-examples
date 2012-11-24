/*
 * bc_factory1.hpp
 *
 *  Created on: Nov 28, 2008
 *      Author: forma
 */
#ifndef BC_CONTAINER_HPP_
#define BC_CONTAINER_HPP_
#include <map>
#include <set>
#include <vector>
#include "bound_cond.hpp"
#include <iostream>
namespace FEM{
//! A simple bc container
//! It uses an extension of a set to store BCs
  class BCContainer : private std::set<BCBase>{
  public:
    //! The actual container
    typedef std::set<BCBase> Container;
    //!\defgroup types Types inherited from set
    //!@{
    typedef Container::iterator iterator;
    typedef Container::const_iterator const_iterator;
    //!	@}
    BCContainer():M_map(){}
    //! \defgroup HelperFunctions Some helper functions
    //!@{
    //! Inserts a bc
    /*! It returns false if the BC was already inserted
        One may also use the standard insert()
        */
    bool addBC(BCBase const &);
    //! It returns the BC associated to a name
    const_iterator getBC(BCName const &) const;
    //! Specialised version if the name is a int
    //! To avoid conversion when the mesh generator uses
    //! an int to mark boundary data.
    const_iterator getBC(int const) const;
    //! It returns all BCs of a given type
    std::pair<const_iterator,const_iterator> getAllBC(BCType type) const;
    //! It prints information about the current BC stored in the container
    void showMe(std::ostream & stream=std::cout) const;
  private:
    /*! To access by name
     * Since we may need to acccess BC by names we store a
     * map to speed up this access
     */
    std::map<BCName,iterator> M_map;
    //! This class is not CopyCOnstructible
    BCContainer(BCContainer const &);
    //! This class is not Assignable
    BCContainer & operator =(BCContainer const &);
  };
}


#endif /* BC_CONTAINER_HPP_ */
