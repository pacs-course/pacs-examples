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

namespace FEM {
  enum BCType {
    Dirichlet, Neumann, Robin, Generic, Other
  };
  
  //! The type used to identify a boundary condition.
  typedef std::string BCName;
  
  //! This struct encapsulates the boundary conditions identifiers.
  /*!
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
  struct BCId{
    BCId(BCType t, BCName n):type(t),name(n){};
    //! the type
    BCType type;
    //! the name
    BCName name;
  };

  //! Utility function which builds an ID.
  inline BCId make_BCId(BCType typ, BCName const & n){return BCId(typ,n);}
  
  //! The type of the function  which imposes the bc (C++11 only).
  typedef std::function<double (double const t, double const * coord)> BcFun;
  
  //! The zero function.
  extern BCFun zerofun;

  //! The one function.
  extern BCFun onefun;
  
  //! A helper function that translates an int into a BCName.
  BCName intToBCName(int i);
  
  //! A class for holding BConditions
  /*!This is a concrete base class to implement boundary conditions in a
   * finite element code. A boundary condition is identified by a type,
   * which is an enum and gives the code the indication on how the bc is applied,
   * a name, which is a string chosen by the user, for instance "wall".
   * A BCBase is uniquely identified by the type and the name.
   * A BCBase contains a vector of indices to "entities", which are tipically
   * the geometry entities to which the bc applies. For a Neumann and Robin
   * boundary contition it will be a list of faces/edges, for a Dirichlet boundary condition a
   * list of nodes. Since the BCBase will be stored in a set we need to make
   * those entities writable on constant objects! So they are declared mutable.
   *
   * A function is recalled by the apply() method, and it implements
   * the boundary condition the function ha as argument the time and the
   * coordinate of a point.
   */
  class BCBase: public BCId
  {
  public:
    //! Constructor using string as names
    explicit  BCBase(BCType t=Dirichlet,
		     BCName n = BCName("Homogeneous"),
		     BcFun fun = zerofun):BCId(t,n),entities_(),fun_(fun){};
    //! Change the identifier
    void set_Id(BCId const & id);
    //! Changes the function
    void set_fun(BCFun const f)const{fun_ = f;}
    // Returns the name
    BCName name()const {return this->name;}
    // Returns the type
    BCType type()const {return this->type;}
    //! Applies boundary condition
    double apply(double const t, double const * coord) const {
      return fun_(t,coord);
    }
    //! It sets the entities to a new value
    void set_entities(std::vector<int> const & e);
    //! It returns the vector of entity index for any use (const version)
    std::vector<int> const & entities() const {return entities_;}
    void showMe(std::ostream & stream=std::cout) const;
  protected:
    std::vector<int> entities_;
    BcFun fun_;
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
