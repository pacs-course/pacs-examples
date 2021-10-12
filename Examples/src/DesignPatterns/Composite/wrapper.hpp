#ifndef WRAPPER_H
#define WRAPPER_H
#include <memory>
/*! \file wrapper.hpp

  The class in this file implements a generic wrapper (hadle) for the
  bridge pattern (i.e. to implement delegation by aggregation using a
  polymorphic hiararchy).  Its role is to ease the memory management of
  object which are used polymorphycally by a class in order to implement a
  rule.  It is a modification of the class presented by Mark Joshi in his
  book "c++ dsign patterns and derivative pricing", Cambridge Press. This
  version mekes use of std::unique_ptr to ease the handling of the pointers.

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

    The root of the problem lies in the fact that the responsability of
    correctly handling the object pointed by a is left to the procedure
    which instantiates UseB, while it should be responsability of UseB
    itself. This is confusing and prone to errror.

    The use of the bridge pattern through a wrapper overcomes the
    problem. A wraper contained in UseB takes care of memory management and
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
    Wrapper<B> Impl;  A wrapper which handles the managment of A COPY
     of the object passed by the constructor, thanks to the method
     clone().
    \endverbatim



    The interesting bit is that by use of implicit conversion and
    overloading of the dereferencing operators * and -> makes the use of
    the wrapper transparent!. The routine that instantiate useB require
    little if no change at all.

*/

/*! The handle (wrapper)

\pre The parameter class T must have a virtual method with the
signature T * clone() const. Beware that T must be the base class and
that the method clone () must be const
*/
template <class T> class Wrapper
{
public:
  Wrapper() : DataPtr(0) {}

  /*! This constuctor takes a reference to an object of type T. It is
    essential for the working of the handle since it defines the
    conversion T& -> Wrapper<T> and thus the conversion from any reference
    to a class derived from T.
  */
  Wrapper(const T &inner) : DataPtr(inner.clone()) {}
  /*! Move constructor defaults

   */
  Wrapper(T &&inner) = default;

  /*!
    The copy constructor grabs a clone of the object stored
    in the other handle
   */
  Wrapper(const Wrapper<T> &original)
    : DataPtr((original.DataPtr.get() != 0) ? original.DataPtr->clone() : 0)
  {}

  /*! Move constructor defaults
   */
  Wrapper &operator=(Wrapper<T> &&) = default;
  /*!
    The assignement operator grabs a clone of the object stored
    in the other handle and resets the current pointer (i.e. it frees
    the memory if required)
   */
  Wrapper &
  operator=(const Wrapper<T> &original)
  {
    if(this != &original)
      DataPtr.reset((original.DataPtr.get() != 0) ? original.DataPtr->clone()
                                                  : 0);
    return *this;
  }

  //! auto_ptr<T> takes care of deleting the data. Nothing to be done!
  ~Wrapper() {}

  //@{
  /*! The handle should work like a pointer to T*/
  inline const T &
  operator*() const
  {
    return *DataPtr;
  }
  inline T &
  operator*()
  {
    return *DataPtr;
  }
  //@}
  //@{
  /*! The handle should work like a pointer to T*/
  inline const T *const
  operator->() const
  {
    return DataPtr.get();
  }
  inline T *
  operator->()
  {
    return DataPtr.get();
  }
  //@}

private:
  std::unique_ptr<T> DataPtr;
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
