/*
 * perfectForwarding.hpp
 *
 *  Created on: Mar 30, 2020
 *      Author: forma
 */

#ifndef EXAMPLES_SRC_TEMPLATES_VARIADICTEMPLATES_PERFECTFORWARDING_HPP_
#define EXAMPLES_SRC_TEMPLATES_VARIADICTEMPLATES_PERFECTFORWARDING_HPP_
#include <memory>
#include <iostream>
namespace apsc
{
//! A smart pointer that implements unique ownership and does deep copy
//! of a clonable class.
//! A more complete version (and different) in CloningUtilities
//!
//! \tparam Base the Base class of the stored resource
  template<class Base>
  class SuperSmart
  {
  public:
    SuperSmart()=default;
    SuperSmart(SuperSmart const & s):MyPointer(s.MyPointer->clone()){};
    SuperSmart(SuperSmart&& s)=default;
    SuperSmart & operator = (const SuperSmart& s)
    {
      if(this != &s)
	MyPointer=s.MyPointer->clone();
      return *this;
    }
    SuperSmart & operator=(SuperSmart&& s)=default;
    Base & operator * (){return *MyPointer;}
    //! A setter taking arbitrary numbers of arguments
    //! and forward them to make_unique, buliding the resource
    //! Existing resourse is deleted
    template <typename Derived,typename... Args>
    void setValue(Args&&... args){
      MyPointer=std::make_unique<Derived>(std::forward<Args>(args)...);
    }
     private:
    std::unique_ptr<Base> MyPointer;
  };
  //! Clonable classes (for the example)
  class Base
  {
  public:
    Base()=default;
    Base(int i):MyI{i}{};
    virtual ~Base()=default;
    virtual std::unique_ptr<Base> clone()const { return std::make_unique<Base>(*this);}
    virtual void showMe()const
    {
      std::cout<<"I am Base and I store "<< MyI<<std::endl;
    }
  protected:
    int MyI=0;
  };

  class Derived: public Base
   {
   public:
    // Inherit constructors of Base
     using Base::Base;
     Derived()=default;
     Derived(int i, double x):Base(i),MyV{x}{};
     virtual std::unique_ptr<Base> clone()const override
     {
       return std::make_unique<Derived>(*this);
     }
     void showMe()const
       {
       std::cout<<"I am Derived and I store "<< MyI<<" "<<MyV<<std::endl;
       }
   protected:
     double MyV=10.0;
   };

}//end namespace apsc


#endif /* EXAMPLES_SRC_TEMPLATES_VARIADICTEMPLATES_PERFECTFORWARDING_HPP_ */
