#ifndef WRAPPER_H
#define WRAPPER_H
#include <memory>
#include <type_traits>
/*!  This class implements a generic wrapper for the bridge pattern. Its
  role is to ease the memory management of object which are used
  polymorphycally by a class in order to implement a rule.  It is a
  modification of the class presented by Mark Joshi in his book "c++ dsign
  patterns and derivative pricing", Cambridge Press. This version mekes use
  of std::unique_ptr to ease the handling of the pointers.
  
  
  We make an example to illustrate the problem. The general structure
  without the wrapper would be
  
  \verbatim
  
  A hierarchy of classes which implements a certain rule
  
  class B;
  
  class D1: public B ......
  
  A class that uses that rule by containement:
  
  class UseB{
  public:
  UseB(B *, ..); The constructor takes a pointer (or a reference)
  to B
  ....
  methods that use B* polymorphically
  ....
  private:
  B * Impl;  A pointer (or reference) to B
  \endverbatim
  
  The problem here is that the scope of an object of type UseB and that
  of the object of the base class B (or possibly reference) passed to the
    constructor are linked. For instance the statement
    
    \verbatim
    
    B* a = new D1;
    UseB foo(a,...)
    delete a;
    \endverbatim
    
    would be invalid because the copy of a stored in foo is now dangling. A
    similar situation happens with references.
    
    The root of the problem lies in the fact that the responsibility of
    correctly handling the object pointed by a is left to the procedure
    which instantiates UseB, while it should be responsibility of UseB
    itself. This is confusing and prone to error.
    
    The use of the bridge pattern through a wrapper overcomes the
    problem. A wrapper contained in UseB takes care of memory management and
    implementing polymorphism by dereferencing a pointer to the base class.

    Here a generic wrapper is implemented as a template class. The templete
    parameter T should be set to the BASE class of the hierarchy. In the
    previous example the situation would change in
    
    \verbatim
  class UseB{
  public:
  UseB(const B &, ...); The constructor takes a reference to B
  ....
  methods that use B* polymorphically
  ....
  private:
  Wrapper<B> Impl;  A wrapper which handles the management of A COPY
  of the object passed by the constructor, thanks to the method
     clone().
     \endverbatim

     \pre The parameter class T must have a virtual method with the
     signature T * clone() const. Beware that T must be the base class and
     that the method clone () must be const.
     

     The interesting bit is that by use of implicit conversion and
     overloading of the dereferencing operators * and -> makes the use of
     the wrapper transparent!. The routine that instantiate useB require
     little if no change at all.
     
*/
template< class T>
class Wrapper
{
public:
  
  
  Wrapper():DataPtr(0){}

  /*! This constructor takes a reference to an object of type T. It is
    essential for the working of the wrapper since it defines the
    conversion T& -> Wrapper<T> and thus the conversion from any reference
    to a class derived from T.
  */

  Wrapper(const T& inner): DataPtr(inner.clone()){}
  
  
  Wrapper(const Wrapper<T>& original):
    DataPtr((original.DataPtr.get() !=0) ? original.DataPtr->clone() : 0){}
  
  Wrapper& operator=(const Wrapper<T>& original)
  {
    if (this != &original)
      DataPtr.reset((original.DataPtr.get() !=0) ? original.DataPtr->clone() : 0);
    return *this;
  }
  
  //! unique_ptr<T> takes care of deleting the data
  
  ~Wrapper()=default;
  
  /*! The Wrapper works like a pointer to T*/
  inline const T& operator*() const{return *DataPtr;}
  inline T&       operator*(){ return *DataPtr;}
  
  /*! The Wrapper works like a pointer to T*/
  inline const T* const operator->() const{ return DataPtr.get();}
  inline T*             operator->(){ return DataPtr.get();}


private:
  std::unique_ptr<T> DataPtr;
};

/*! A version of the wrapper that works also on non clonable classes.
 * We need that D derives from B (it is a useful setting for the isDerived interrogator)
 * Moreover D must be assignable and copy-constructible.
 */
template< class B, class D>
class WrapperNC: protected Wrapper<B>
{
public:
  static_assert(std::is_base_of<B,D>::value,
		"Template classes must be in a i-a relation");
  WrapperNC(const B& inner){this->DataPtr.reset(this->clone(inner));}
  
  WrapperNC(const WrapperNC<B,D>& original){
    this->DataPtr(original->clone());}
  
  WrapperNC& operator=(const WrapperNC<B,D>& original)
  {
    if (this != &original) this->DataPtr.reset(original->clone());
    return *this;
  }
  
  /*! WrapperNC does not add variables to Wrapper
   * so there is no need to declare the destructor
   * of Wrapper virtual
   */
  ~WrapperNC()=default;
  
  inline const B& operator*() const{return *(this->DataPtr);}
  inline B&       operator*(){ return *(this->DataPtr);}
  
  /*! The Wrapper works like a pointer to B  */
  inline const B* const operator->() const{ return this->DataPtr.get();}
  inline B*             operator->(){ return this->DataPtr.get();}
  
private:
  B * clone(D const & a){ return new D(a);}
  B * clone(){ return (this->DataPtr.get() !=0)?
      new D(*(this->DataPtr)): 0;}
};
#endif


/*
 *
 * Copyright (c) 2002
 * Mark Joshi
 * Copyright (c) 2006
 * Luca Formaggia (version with unique_ptr<T>)
 *
 * Permission to use, copy, modify, distribute and sell this software for
 * any purpose is hereby granted without fee, provided that the above
 * copyright notice appear in all copies and that both that copyright
 * notice and this permission notice appear in supporting documentation.
 * Mark Joshi makes no representations about the suitability of this
 * software for any purpose. It is provided "as is" without express or
 * implied warranty.
*/

