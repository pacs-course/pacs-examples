#ifndef __NONLINSYS_HPP__
#define __NONLINSYS_HPP__
#include <functional>
#include <vector>
#include <Eigen/Dense>
namespace NonLinearSystems{
  //! It implements a non linear system of eqns \f$F(x): R^N\to R^N \f$ 
  /*! We have chosen to have a dynamic system: equations may be added/changed
    (but not deleted) run-time. An alternative is to use templates 
    (see NonLinearSystam.hpp)
  */
  class NonLinSys{
  public:
    //! The type used for the argument of the function.
    /*!
       It must conform to a dynamic vector: have [] operators;
     */
    using argumentType=Eigen::VectorXd;
    //! The type used for as return value of the call operator.
    /*!
       It must conform to a dynamic vector: have [] operator and
       contructor taking an integer.
     */
    using  returnType=Eigen::VectorXd;
    //! type of the contained functions 
    using funType=std::function<double (argumentType const &)>;

    //! Default construtor (NO NEED TO SPECIFY THEM)
    /*!
      I use the default synthetic constructor
     */
    // NonLinSys()=default;
    //! I use the synthetic copy constructor
    // NonLinSys(NonLinSys const &)=default;
    //! I use the synthetic copy-assignment.
    /*!  
      The default declaration is a C++11 extension and it is not
      strictly necessary. If we do not use it we have the synthetic
      operator all the same. However it helps the user of the class
      since it clarifies the intention of the developer.
     */
    // NonLinSys & operator =(NonLinSys const &)=default;

    //! Compute \f$ -F(x) \f$
    returnType residual(argumentType const & x) const;
    //! Compute \f$ F(x) \f$.
    returnType operator()(argumentType const & x) const;
    //! Add a function to the system.
    void addToSystem(funType const & f);
    //! Number of equations.
    std::size_t numEq()const {return M_funs.size();}
    //! Returns the i-th function.
    funType & operator [](unsigned int i){return M_funs[i];}
    //! Returns the i-th function.
    funType  operator [](unsigned int i) const {return M_funs[i];}
    //! Returns \f$ ||f(x)|| \f$
    double norm(argumentType const & x) const;
  private:
    std::vector<funType> M_funs; 
  };
    
} // End of namespace
#endif
