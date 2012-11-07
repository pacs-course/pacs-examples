#ifndef __NONLINSYS_HPP__
#define __NONLINSYS_HPP__
#include <functional>
//#include <vector>
#include <Eigen/Dense>
namespace NonLinearSystems{
  //! The type used for the argument of the function.
  /*!
    It must conform to a dynamic vector: have [] operators;
   */
  typedef Eigen::VectorXd argumentType;
  //! The type used for as return value of the call operator. 
  /*!
    It must conform to a dynamic vector: have [] operator and
    contructor taking an integer.

   */
  typedef Eigen::VectorXd returnType;
  //! type of the contained functions (C++11 extension)
  typedef std::function<double (argumentType const &)> funType;
  
  //! It implements a non linear system in the form \f$F(x)=0$ 
  /*! We have chosen to have a dynamic system: equations may be added/changed
    (but not deleted) run-time. An alternative is to use templates. 
  */
  class NonLinSys{
  public:
    //! Default construtor
    /*!
      I use the default synthetic constructor (C++11).
    */
    NonLinSys()=default;
    //! I use the synthetic copy constructor (C++11).
    NonLinSys(NonLinSys const &)=default;
    //! I use the synthetic copy-assignment.
    /*!  
      The default declaration is a C++11 extension and it is not
      strictly necessary. If we do not use it we have the synthetic
      operator all the same. However it helps the user of the class
      since it clarifies the intention of the developer.
    */
    NonLinSys & operator =(NonLinSys const &)=default;
    
    //! Compute \f$ -F(x) \f$
    returnType residual(argumentType const & x) const;
    //! Compute \f$ F(x) \f$.
    returnType operator()(argumentType const & x) const;
    //! Add a function to the system.
    void addToSystem(funType const & f);
    //! Number of equations.
    unsigned int numEq()const;
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
