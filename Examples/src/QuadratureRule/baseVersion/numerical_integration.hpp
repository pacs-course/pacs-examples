#ifndef NUMERICAL_INTEGRATION_HPP
#define NUMERICAL_INTEGRATION_HPP
#include "QuadratureRuleBase.hpp"
#include "mesh.hpp"
namespace NumericalIntegration{
  using namespace Geometry;

  /*!
    Class for composite integration.
    
    It is implemented using the following design:
    - Composition with a QuadraturRule is implemented via a
      unique_ptr<>. A Quadrature object thus owns a polymorphic object
      of type QuadratureRule. This allows to assign the quadrature rule
      run-time.

    - A 1D mesh is simply aggregated as an object. So a copy is
      made. We however support move semantic (introduced in C++11) so that
      if the mesh class implements a move constructor we can move the
      mesh into the Quadrature, saving memory.

      It is an example of the Bridge Design pattern: part of the implementation
      is delegated to a polymorphic object (the QuadratureRule).
   */
  class Quadrature
  {
  public:
    typedef NumericalIntegration::FunPoint FunPoint;
    //!Constructor.
    /*!  
      \param rule A unique_ptr storing the rule.  
      \param mesh The 1D mesh (passed via universal reference)
     */
    template <typename MESH> 
    Quadrature(QuadratureRuleHandler && rule, MESH && mesh):
      _rule(std::move(rule)),_mesh(std::forward<MESH>(mesh)){}
      
    //!Constructor that takes a const reference (I need to copy)
    /*!  
      Here  I take advantage of clone!
      \param rule A unique_ptr storing the rule.  
      \param mesh The 1D mesh (passed as universal reference)
    */
    template <typename MESH> 
    Quadrature(QuadratureRuleHandler const & rule, MESH&& mesh):
      _rule(rule->clone()),_mesh(std::forward<MESH>(mesh)){}
    //!A second constructor
    /*!  In this case we pass the object and we use the fact that the
      QuadratureRule classes are Clonable (that is they contain a
      clone() method). So, I can safely pass also a reference to a
      QuadratureRule base class.  If QuadratureRule where not clonable
      it had be better not to pass a reference but an object.  The
      implementation would be different and I could only pass concrete
      object types (not the abstract class!): 
      \code template <class T>
      Quadrature(const T rule, Mesh1D& mesh):
      Quadrature(QuadratureRuleHandler(new T(rule)),mesh){} 
      \endcode

      \param rule A unique_ptr storing the rule.
      \param mesh The 1D mesh
     */ 
    template <typename MESH> 
    Quadrature(const QuadratureRule & rule, MESH&& mesh):
      _rule(rule.clone()),_mesh(std::forward<MESH>(mesh)){}
    //! Copy constructor.
    Quadrature(Quadrature const & rhs): _rule(rhs._rule->clone()),  _mesh(rhs._mesh){}
    //! Move constructor.
    Quadrature(Quadrature&& rhs): _rule(std::move(rhs._rule)), _mesh(std::move(rhs._mesh)){}
    //! Copy assignment.
    Quadrature & operator=(Quadrature const &);
    //! Move assignment.
    Quadrature & operator=(Quadrature&&);
    //! Calculates the integal on the passed integrand function.
    double apply(FunPoint const &) const;
    QuadratureRule const & myRule()const {return *(_rule.get());}
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
