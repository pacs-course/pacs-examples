/*
 * main.cpp
 *
 *  Created on: Apr 18, 2019
 *      Author: forma
 */
#include<iostream>
#include "../CloningUtilities/CloningUtilities.hpp"

using namespace Utility;
/*
 * This is the version where we implement clone() directly as a method of the class hierarchy
 *
class Base
{
public:
  Base(double x=0.0):M_n(x){}
  virtual void print()const {std::cout<<"Base with value "<< M_n<<"\n";}
  virtual ~Base()=default;
  void setN(double const & x){M_n=x;}
  virtual std::unique_ptr<Base> clone() const {return std::make_unique<Base>(*this);}
protected:
  double M_n;
};

class Derived final: public Base
{
public:
  Derived(double x=0.0):Base(x){}
  void print() const override {std::cout<<"Derived with value "<<this->M_n<<"\n";}
  virtual std::unique_ptr<Base> clone() const override {return std::make_unique<Derived>(*this);}
};
*/

//! Version that uses the Clonable utility
class Base : public Clonable<Base,Base>
{
public:
  Base(double x=0.0):M_n(x){}
  virtual void print()const {std::cout<<"Base with value "<< this->M_n<<"\n";}
  virtual ~Base()=default;
  void setN(double const & x){M_n=x;}
protected:
  double M_n;
};

//! Version that uses the Clonable utility
class Derived final: public Base, public Clonable<Base,Derived>
{
public:
  Derived(double x=0.0):Base{x}{}
  void print() const override {std::cout<<"Derived with value "<<this->M_n<<"\n";}
};

//! This class composes with an object of the polymorphic family
class Composer
{
public:
  //! Attaches the component. Here if I can move I move
  /*!
   * \tparam T Base or a class derived by Base
   */
  template<class T>
  void compose(T&& b)
  {
    ptr=std::forward<T>(b);
  }

  void print()
  {
    std::cout<<" composed with ";
    ptr->print();
  }
private:
  //! Cloning is taken care by the class Wrapper
  Wrapper<Base> ptr;
};

int main()
{
  Composer ca;
  {
    Derived a;
    a.setN(35.0);
    ca.compose(a);
    /* Now I can delete a safely
     it has been cloned into ca
     I could have also made ca.compose(std::move(a));
    */
  }
  ca.print();
// But I can just create the object
  //and pass it: it will be moved.
  Composer cb;
  cb.compose(Derived{46.0});
  cb.print();
  Composer cc(cb);
  cc.print();
}





