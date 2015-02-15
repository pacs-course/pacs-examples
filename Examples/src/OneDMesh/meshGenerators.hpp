#ifndef HH_GENERATOR_HH
#define HH_GENERATOR_HH
#include <vector>
#include <functional>
#include <stdexcept>
#include "domain.hpp"
namespace Generator{
  using MeshNodes=std::vector<double>;
  /*! \defgroup meshers Functors which generates a 1D mesh.
    @{ */
  //! Uniform mesh
  class Uniform
  {
  public:
  /*! constructor
    @param domain A 1D domain
    @param b num_elements Number of elements
  */
    Uniform(Geometry::Domain1D const & domain, unsigned int const num_elements):
      M_domain(domain),M_num_elements(num_elements){}
    //! Call operator
    /*!
      @param meshNodes a mesh of nodes
     */
    MeshNodes operator()();
  private:
    Geometry::Domain1D M_domain;
    std::size_t M_num_elements;
  };
   
  //! variable mesh size
  /*! 
    The class accepts a spacing function h=h(x) which should return a strictly 
    positive value
    for all x in the domain: \f$ h(x)\ge \alpha>0, \quad x\in\Omega \f$.
    The mesh is generated so that
    \f[
    x_i=\beta\int_0^i h(s)^{-1}ds
    \f]
    where \f$\beta\f$ 
  */
  class VariableSize
  {
  public:
    /*!
      @brief Constructor
          @param domain A 1D domain
	  @param h spacing function
	  @param max_num_elements Maximum number of elements
     */
    VariableSize(const Geometry::Domain1D & domain, 
		 std::function<double (double const & x)> const & h, 
		 const unsigned int max_num_elements):
      M_domain(domain),M_h(h),M_num_elements(max_num_elements){}
    MeshNodes operator()();
  private:
    Geometry::Domain1D M_domain;
    std::function<double (double const & x)> const & M_h;
    std::size_t M_num_elements;;
    
  };
  /*! @}*/
}
#endif
