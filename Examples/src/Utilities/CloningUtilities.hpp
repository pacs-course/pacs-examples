#ifndef WRAPPER_H
#define WRAPPER_H
#include <memory>
#include <type_traits>
#include <cassert>
namespace apsc
{
  /* wrapper namespace. To use this utilities you need
     apsc::TypeTraits, to reduce possibility
     of name clashes
  */
  namespace TypeTraits
  {
    //! A template that takes anything
    /*!  Void<T>::type is always equal to void.  It is used as a way
      of checking that a type has been defined, irrespectively from
      the actual type.  Note that typename is not followed by the neme
      of the parameter.  It is
    */
    template<typename> struct Void { using type=void; };
  }
    /*!  Primary template for a class that checks if a clss has the method clone

      The second argument is used to
      activate SFINAE.  It inherits from false_type, so it
      represent "false" Remember that std::false_type is convertible to
      a boolean equal to false.
     */
    template<typename T, typename Sfinae = void>
    struct has_clone: public std::false_type {};

    /*!  Specialised version that is activated if T is clonable.

      Indeed, if T is not clonable the second template parameter cannot
      be substituted with a valid type. So SFINAE applies and this
      version is discarded. Note that it inherits from std::true_type.

      declval<T&>() allows to test the return type of clone() (the
      result of the test is irrelevant in this case) with no need of
      creating an object of type T. Moreover, we use T& because T may be
      a polymorphic object, and clone() may be defined in the base
      class.
    */
    template<typename T>
    struct has_clone<
      T,
      typename TypeTraits::Void<
        decltype( std::declval<T&>().clone() )
               >::type
      >: std::true_type {};

    //! A helper function
    /*!
     *  It returns true if the class is clonable
     *  \tparam T the type to check
     */
    template <class T>
    constexpr bool isClonable() { return has_clone<T>(); }

  /*! A smart pointer that handles cloning for compusing with polymorphic objects
   *
   This class implements a generic wrapper around a unique pointer
   useful to support the bridge pattern. Its role is to ease the memory
   management of object which are composed polymorphycally in a class
   in order to implement a rule.  It is a extensive modification of the
   class presented by Mark Joshi in his book "c++ design patterns and
   derivative pricing", Cambridge Press. This version makes use of
   std::unique_ptr to ease the handling of memory.
   
   It handles memory as a unique_ptr, but implements copy operations by
   cloning the resource.
   
   We make an example to illustrate the problem. Let's consider first a
   solution with aggregation
   
   \code
   
   //A hierarchy of classes which implements a certain rule
   
   class B;
   
   class D1: public B ......
   
   A class that uses an object of the B hierarchy by aggregation:
   
   class UseB{
   public:
   UseB(B *); The constructor takes a pointer (or a reference)
   to B
   ....
   //methods that use B* polymorphically
   ....
   private:
   B * Impl;  //A pointer (or reference) to B
   \endcode
   
   The problem here is that the scope of an object of type UseB and
   that of the object of the base class B (or possibly reference)
   passed to the constructor are linked. For instance the statements
   
   \code
   B* a = new D1;
   UseB foo(a)
   delete a;
   \endcode
   
   would be invalid because the copy of a stored in foo is now
   dangling. A similar situation happens with references.  Moreover,
   the state of UseB depends on the state of a! And that is not what
   we want here, we want the object pointed by Impl to be part of
   class UseB: we want composition not aggregation!
   
   But how to compose a polymorphic object? A possibility is to add
   to B and to D1 the method clone()
   
   \code
   virtual std::unique_ptr<T> clone() const {return std::make_unique<D>(*this);}
   \endcode
   
   Then, if useB::Impl is now a unique_ptr<B>, the constructor of useB may do
   \code
   useB(const B&):Impl(B.clone()){}
   \endcode
   
   But now we have to delegate to the class B the handling of the
   "cloning", by writing also specific copy constructors etc.
   
   Isn't it possible to have a "supersmart" pointer to which delegate
   all the cloning business, so that the class UseB just needs to
   store one of those "supersmart" pointer? What I want is something
   that acts as a unique_ptr with respect to memory management (it is
   an owning pointer: pointer destruction implies destruction of the
   pointed resource). But I want to introduce a COPY SEMANTIC (which
   is deleted in unique_ptr) that does a DEEP COPY of the resource
   exploiting the clone() method.
   
   To this purpose, we design a wrapper around a unique_ptr. The
   wrapper behaves as a pointer and takes care of memory management,
   like a unique_ptr, but it exploit clonable classes to implement
   polymorphic composition.
   
   Here a generic wrapper is implemented as a template class. The templete
   parameter T should be set to the BASE class of the hierarchy.
   
   In the previous example the situation would change in
   
   \code
   class UseB{
   public:
   UseB(const B &):Impl(B); The constructor takes a reference to B
   ....
   methods that use B* polymorphically
   ....
   private:
   // I use the wrapper that behaves like a pointer!
   Wrapper<B> Impl;
   \endcode
   
   
   \pre The parameter class T must have a virtual and constant
   clone() method with the indicated signature.
   
   The interesting bit is that by use of implicit conversion and
   overloading of the dereferencing operators * and -> makes the use
   of the wrapper transparent!.
   
   @tparam T the base class
  */
  template< class T>
  class Wrapper
  {
  public:
    //! Check if clone is present
    static_assert(isClonable<T>(),"template parameter of Wrapper must be a clonable class. clone() should return a unique_ptr");
    // The type of the stored pointer
    using Ptr_t=std::unique_ptr<T>;
    //! The default constructor
    /*
     * The synthetic one is ok since the default constructor of a unique_ptr
     * sets it to the null pointer.
     */
    Wrapper()=default;
    
    /*! This constructor takes a reference to an object of type T or derived  from T. It is
      essential for the working of the wrapper since it defines the
      conversion T& -> Wrapper<T> and thus the conversion from any reference
      to a class derived from T. It uses clone() to clone the resource.
    */
    Wrapper(const T& inner): DataPtr(inner.clone()){}
    
    //! Taking a rvalue reference to a unique_pointer
    /*!
     * I can just move the stored pointer!
     */
    Wrapper(Ptr_t&& p):DataPtr(std::move(p)){}
    
    //! Copy constructor
    /*!
     * Here I implement a copy semantic that makes deep copy of the resource.
     * I am sure that I will obtain the correct object of the hierarchy
     * thanks to clone().
     *
     * I exploit conversion unique_ptr -> bool  and the fact thet the default constructor
     * of a unique_ptr initializes it with the null pointer.
     */
    Wrapper(const Wrapper<T>& original):
      DataPtr(original.DataPtr ? original.DataPtr->clone() : Ptr_t{}){}
    //!copy-assignement operator
    /*!
     * It resets current resource and clone that of the other wrapper
     */
    Wrapper& operator=(const Wrapper<T>& original)
    {
      if (this != &original)
        DataPtr=(original.DataPtr ? original.DataPtr->clone() : Ptr_t{});
      return *this;
    }
    //! Maybe I want to assign a unique pointer
    /*!
     * If argument is an rvalue unique_ptr can be moved.
     */
    Wrapper& operator=(Ptr_t&& p)
    {
      DataPtr=std::move(p);
      return *this;
    }
    //! Assignment a unique pointer by cloning
    /*!
     *  If argument is an lvalue I need clone()
     */
    Wrapper& operator=(const Ptr_t& p)
    {
      DataPtr=p->clone(); // note that this is a move-assignment
      return *this;
    }
    
    //! The move constructor
    /*!
     * The synthetic one is ok since unique_ptr can be moved
     */
    Wrapper(Wrapper<T>&&)=default;
    
    //! Move assignement
    Wrapper& operator=(Wrapper<T>&&)=default;
    
    //! unique_ptr<T> takes care of deleting the data
    ~Wrapper()=default;
    
    /*! The Wrapper works like a pointer to T*/
    const T& operator*() const{return *DataPtr;}
    T&       operator*(){ return *DataPtr;}
    
    /*! The Wrapper works like a pointer to T*/
    const T* const operator->() const{ return DataPtr.get();}
    T*             operator->(){ return DataPtr.get();}
    /*! This class imitates that of the uniqe pointer so it exposes the same member types*/
    using pointer= typename Ptr_t::pointer;
    using element_type =T;
    using deleter_type = typename Ptr_t::deleter_type;
    /* The method release */
    auto release(){return DataPtr.release();}
    /*! reset. You can pass directly a pointer */
    void reset(pointer ptr = pointer()){DataPtr.reset(ptr);}
    /*! swap wrappers */
    void swap(Wrapper<T>& other) noexcept {DataPtr.swap(other.DataPtr);}
    /*! get the pointer */
    pointer get() const noexcept { return DataPtr.get();}
    /*! conversion to bool */
    explicit operator bool() const noexcept {return static_cast<bool>(DataPtr);}
  private:
    Ptr_t DataPtr;
  };
  //! Utility to make a Wrapper 
  /*!
    Creates a Wrapper<Base> indicating Base and Derived class. 
    I need two compulsory template parameters, one for the Base
    and une for the concrete Derived class since I have not implemented
    the conversion Wrapper<D> to Wrapper<B>.
   
    /tparam B The base class. The function returns Wrapper<B>
    /tparam D The derived class. The Wrapper will own an object of type D.
    /tparam Args Automatically deduced possible arguments for the constructor of D
    /param args The (possible) arguments of type Args
    /todo Conversion operator between Wrapper<D> and Wrapped<B>. 
     It should not be so difficult and it would simplify things a little.
   */
  template<class B, class D, typename... Args>
  Wrapper<B> make_Wrapper(Args&&... args)
  {
    return Wrapper<B>(std::make_unique<D>(std::forward<Args>(args)...));
  }
}// end namespace apsc

#endif
/*
 *
 * Copyright (c) 2002
 * Mark Joshi Copyright (c) 2006
 * Luca Formaggia (modifications and additions)
 *
 * Permission to use, copy, modify, distribute and sell this software for
 * any purpose is hereby granted without fee, provided that the above
 * copyright notice appear in all copies and that both that copyright
 * notice and this permission notice appear in supporting documentation.
 * Mark Joshi and Luca Formaggia make no representations about 
 * the suitability of this
 * software for any purpose. It is provided "as is" without express or
 * implied warranty.
*/

