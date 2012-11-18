#ifndef NUMERICAL_INTEGRATION_HPP
#define NUMERICAL_INTEGRATION_HPP
#include "QuadratureRule.hpp"
#include "mesh.hpp"
namespace NumericalIntegration{
  using namespace Geometry;

  /*!
    Class for composite integration.
    
    It is implemented using the following design:
    - Composition with a QuadraturRule is implemented via a
      unique_ptr. A Quadrature object thus owns a polymorphic object
      of type QuadratureRule. This allows to assign the quadrature rule
      run-time.
    - The 1D mesh is simply aggregated as an object. So a copy is
      made. We however support move semantic (introduced in C++11) so that
      if the mesh class implements a move constructor we can move the
      mesh into the Quadrature, saving memory.

      It is an example of the Bridge Design pattern: part of the implementation
      is delegated to a polymorphic object (the QuadratureRule)
   */
  class Quadrature
  {
  public:
    typedef NumericalIntegration::FunPoint FunPoint;
    //!Constructor.
    /*!  
      \param rule A unique_ptr storing the rule.  
      \param Mesh1D The 1D mesh 
     */
    Quadrature(QuadratureRuleHandler rule, Mesh1D& mesh);
    //!Constructor.
    /*!  
      \param rule A unique_ptr storing the rule.  
      \param Mesh1D The 1D mesh 

      The mesh is passed as a rvalue reference (C++11
      only). This allows, if the mesh class implements move semantic
      and if I pass a temporary, to move the object without temporaries.
     */
    Quadrature(QuadratureRuleHandler rule, Mesh1D&& mesh);
    //!A second constructor
    /*!
      In this case we pass the object and we use the fact that the
      QuadratureRule classes are Clonable. That is they contain a 
      clone() method. So I can safely pass also a reference
      to a QuadratureRule base class. If QuadratureRule where not
      clonable it had be better not to pass a reference but an object.
      The implementation would be different and I could only pass concrete
      object types (not the abstract class!):
      \code
      template <class T>
        Quadrature(const T rule, Mesh1D& mesh):
      Quadrature(QuadratureRuleHandler(new T(rule)),mesh){}
      \endcode

      \param rule A unique_ptr storing the rule.
      \param Mesh1D The 1D mesh
     */ 
    template <class T>
    Quadrature(const T & rule, Mesh1D& mesh):
      Quadrature(QuadratureRuleHandler(rule.clone()),mesh){}
    //! Moving the mesh
    template <class T>
    Quadrature(const T & rule, Mesh1D&& mesh):
      Quadrature(QuadratureRuleHandler(rule.clone()),std::move(mesh)){}


    //! Copy constructor
    Quadrature(Quadrature const &);

    //! Copy assignment
    Quadrature & operator=(Quadrature const &);

    double apply(FunPoint const &) const;
  protected:
    QuadratureRuleHandler _rule;
    Mesh1D _mesh;
  };

  /*
    (-1,1) ->(a,b) y*(b-a)/2 + (a+b)/2
    wfactor dx/dy = h/2 
   */

    
  
}



#endif
