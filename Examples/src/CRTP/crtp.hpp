#ifndef  __HHH_CRTP_HH__
#define  __HHH_CRTP_HH__
#include <iostream>
//! @file crtp.hpp
//! @brief An example of Curiusly recurring template patterns

//! Base class
template<typename T>
class Base{
 public:
  //! Delegates to derived class
  void fun(double const & x){
    leaf().fun(x);}
  //! Example with a static function
  static void fstatic(){T::fstatic();}
private:
  //! Returns myself as derived
  T & leaf(){return static_cast<T &>(*this);}
};

//!Derived class
class Derived: public Base<Derived>
{
public:
  //!Implementation of function fun
  void fun(double const & x){
    std::cout<<"In the Derived. x="<<x<<std::endl;
  }
  //! Implementation of the static function.
  static void fstatic(){std::cout<<"Static function in derived"<<std::endl;}
};
//!Derived class
class Derived2: public Base<Derived2>
{
public:
  //!Implementation of function fun
  void fun(double const & x){
    std::cout<<"In Derived  x="<<x<<std::endl;
  }
  //! Implementation of the static function.
  static void fstatic(){std::cout<<"Static function in derived2"<<std::endl;}
};

#endif
