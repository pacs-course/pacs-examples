#ifndef HH_jacob_HH
#define HH_jacob_HH
#include <limits>
#include "nonLinSys.hpp"
namespace NonLinearSystems{
  //! The type of a jacobian
  typedef Eigen::MatrixXd jacobianType;
  //! This is a base class for Jacobian computations
  class Jacobian{
  public:
    virtual jacobianType operator()(argumentType const & x) const=0;
    virtual ~Jacobian(){};
  };

  //! Computes the jacobian by finite differences.
  /*!
    The final keyword has been introduced in C++11 to stop
    a hierarchy. No class can be derived from it
   */
  class DiscreteJacobian final: public Jacobian 
  {
  public:
    //! Constructor optionally takes the pointer to a non linear system.
    DiscreteJacobian(NonLinSys * sys=nullptr):M_sys(sys),
					      M_tol(defaultTol){};
    //! Sets the non linear system.
    void setNonLinSys(NonLinSys * s){M_sys=s;};
    //! Returns the non linear system.
    NonLinSys * getNonLinSys()const {return M_sys;};
    //! Sets the tolerance for computation of Jacobian
    void setTol(double const tol){M_tol=tol;}
    //! Returns the tolerance currently used.
    double getTol()const {return M_tol;}
    //! Computes the Jacobian using finite diffrences.
    jacobianType operator()(argumentType const & x)const;
    //! Default destructor (C++11).
    ~DiscreteJacobian()=default;
  private:
    //! The non-linear system, stored as a pointer.
    NonLinSys * M_sys;
    //! The current tolerance.
    double M_tol;
  public:
    //! Default tolerance.
    static double const defaultTol;
  };
  
  //! Initialization of the default tolerance.
  double const 
  DiscreteJacobian::defaultTol(1.e6*std::numeric_limits<double>::epsilon());
  

}
#endif
