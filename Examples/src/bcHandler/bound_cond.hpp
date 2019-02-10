/*
 * bound_cond.hpp
 *
 *  Created on: Nov 28, 2008
 *      Author: forma
 */

#ifndef BOUND_COND_HPP_
#define BOUND_COND_HPP_
#include <string>
#include <vector>
#include <iostream>
#include <functional>

  /*! \file bound_cond.hpp
   *  Boundary condition identifiers are formed by a BCType entry,
   *  which select the type of boundary conditions, and a string,
   *  which gives a name of the BC within that type.  For instance:
   *  BCId may contain Dirichlet and "Wall" which identifies the
   *  boundary condition "Wall" of being of Dirichlet type. We will
   *  not allow two BC with the same name, even if they are of
   *  different type The class is a template to allow different
   *  definitions of BCName. Indeed, in many implementations we
   *  require that BCName be just an int. The important thing is that
   *  a BCName should have an ordering since we may use it later as a
   *  key.
   */

namespace FEM {

  //! The type. None means no type yet assigned
  enum BCType { Dirichlet=1, Neumann=2, Robin=3, Other=4, None=99};
  //! The name used to identify a boundary condition.
  using BCName=std::string;
  //! The type of the function  which imposes the bc (C++11 only).
  using BCFun=std::function<double (double const t, double const * coord)>;

  //! The zero function.
  extern BCFun zerofun;

  //! The one function.
  extern BCFun onefun;

  //! A helper function that translates a sting into a BCType.
  BCType stringToBCType(BCName const & s);
  
  //! Type of the identifiers holding the index of the objects where the bc is imposed
  using Id=std::size_t;
  

  //! A class for holding BConditions
  /*!
   * This is a concrete base class to implement boundary conditions in a
   * finite element code. A boundary condition is identified by a type,
   * which is an enum and gives the code the indication on how the bc is applied,
   * a name, which is a string chosen by the user, for instance "wall".
   * A BCBase is uniquely identified by the type and the name.
   * A BCBase contains a vector of indices to "entities", which are tipically
   * the geometry entities to which the bc applies.
   *
   * For a Neumann and Robin boundary condition it will be a list of faces/edges,
   * for a Dirichlet boundary condition a list of nodes.
   * Since the BCBase will eventually be stored in a set we need to make
   * those entities writable on constant objects! So they are declared mutable.
   * Of course the ordering operator used for the set does not depend on the entities,
   * that's why we can do this safely.
   *
   * A function is recalled by the apply() method, and it implements
   * the boundary condition the function ha as argument the time and the
   * coordinate of a point.
   */
  class BCBase
  {
  public:
    //! Constructor using string as names
    explicit  BCBase(BCType t=None,
		     BCName n = BCName("Homogeneous"),
		     BCFun fun = zerofun):
      M_t(t),
      M_name(n),
      M_fun(fun){};
    //! Change the name
    void set_name(BCName const & n)
    {
      M_name=n;
    }
    //! Change the type
    void set_type(BCType const & t)
    {
      M_t=t;
    }
    //! Changes the function
    void set_fun(BCFun const & f)
    {
      M_fun=f;
    }
    //! It sets the entities to a new value
    /*
      \param e Any standard container of Ids that can be assigned to a vector<Id>
     */
    template<typename EntityList>
    void set_entities(EntityList const& e)
    {
      M_entities.assign(e.cbegin(),e.cend());
    }

    // Returns the name
    BCName name()const {return this->M_name;}
    // Returns the type
    BCType type()const {return this->M_t;}
    //! Returns the vector of entity index for any use (const version)
    std::vector<int> const & entities() const {return M_entities;}
    //! Applies boundary condition
    double apply(double const t, double const * coord) const;
    std::ostream & showMe(std::ostream & stream=std::cout) const;
  protected:
    BCType M_t;
    BCName M_name;
    BCFun M_fun;
    std::vector<int> M_entities;
  };
  
  //! A predicate to test if a bc has a given type.
  /* 
     To extract from a container all the bc of a certain type.
  */
  class isBCTypeEqual{
  public:
    explicit isBCTypeEqual(BCType t):type_(t){};
    bool operator() (const BCBase & b){return b.type()==type_;}
  private:
    BCType type_;
  };
  
  //! A predicate to test if a bc has a given name.
  /* 
     Useful to extract from a container all the bc with a given name.
  */
  class isBCNameEqual{
  public:
    explicit isBCNameEqual(BCName n):name_(n){};
    bool operator() (const BCBase & b){return b.name()==name_;}
  private:
    BCName name_;
  };
}
#endif /* BOUND_COND_HPP_ */
