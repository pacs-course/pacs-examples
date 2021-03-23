#ifndef CLONINGANDPOINTERWRAPPER_H
#define CLONINGANDPOINTERWRAPPER_H
#include <functional>
#include <memory>
#include <type_traits>
namespace apsc
{
  /*
   * @note To use this utilities you need apsc::TypeTraits,
   */
  namespace TypeTraits
  {
    /*!
     * A type trait than checks if your class contains a method called clone() const
     * that returns a unique pointer convertible to the class specified in the
     * template parameter. I use it to implement clonable classes which enable the
     * prototype design pattern on a polymorphic family of classes. This is the
     * primary template, which maps to false
     * @tparam T
     * @tparam Sfinae
     */
    template <typename T, typename Sfinae = void>
    struct has_clone : public std::false_type
    {};

    /*!  Specialised version that is activated if T is clonable.

  Indeed, if T is not clonable the second template parameter cannot
  be substituted with a valid type. So SFINAE applies and this
  version is discarded. Note that it inherits from std::true_type.

  declval<T&>() allows to test the return type of clone() with no need of
  creating an object of type T.

 @tparam T the base class of the hierarchy of clonable classes
     */
    template <typename T>
    struct has_clone<
    T, typename std::enable_if<std::is_convertible_v<
    std::unique_ptr<T>, decltype(std::declval<const T &>().clone())> >::type>
    : std::true_type
      {};

    //! A helper function
    /*!
     *  It returns true if the class is clonable
     *  \tparam T T the base class of the hierarchy of clonable classes
     */
    template <class T>
    constexpr bool
    isClonable()
    {
      return has_clone<T>();
    }

    /*!
     * C++17 style for extracting the value of type trait. It is true if T is
     * clonable.
     * @tparam T the base class of the hierarchy of clonable classes
     */
    template <typename T> constexpr bool has_clone_v = isClonable<T>();
  } // end namespace TypeTraits

 /*! A smart pointer that handles cloning for compusing with polymorphic objects
  
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
 virtual std::unique_ptr<B> clone() const {return std::make_unique<D>(*this);}
 \endcode

 Then, if useB::Impl is now a unique_ptr<B>, the constructor of useB may do
 \code
 useB(const B&):Impl(B.clone()){}
 \endcode

 But now we have to delegate to the class B the handling of the
 "cloning", by writing also specific copy constructors etc.

 Is it possible to have a "supersmart" pointer to which delegate
 all the cloning business, so that the class UseB just needs to
 store one of those "supersmart" pointers? What I want is something
 that acts as a unique_ptr with respect to memory management (it is
 an owning pointer: pointer destruction implies destruction of the
 pointed resource). But I want to introduce  COPY SEMANTIC (which
 is deleted in std::unique_ptr) that does a DEEP COPY of the resource
 exploiting the clone() method.

 To this purpose, we design a wrapper around a unique_ptr. The
 wrapper behaves as a pointer and takes care of memory management,
 like a unique_ptr, but it exploit clonable classes to implement
 polymorphic composition.

 Here a generic wrapper is implemented as a template class. The templete
 parameter T should be set to the BASE class of the hierarchy.
 The interesting bit is that by use of implicit conversion and
 overloading of the dereferencing operators * and -> makes the use
 of the wrapper transparent!.
 
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
 PointerWrapper<B> Impl;
 \endcode


 \pre The parameter class T must have a virtual and constant
 clone() method with the indicated signature.

 @tparam T the base class
   */
  template <class T> class PointerWrapper
  {
  public:
    // Check if clone is present
    static_assert(TypeTraits::isClonable<T>(),
                  "template parameter of Wrapper must be a clonable class. "
                  "You must have a virtual clone()const method that returns a unique_ptr<T>");
    // The type of the stored unique pointer
    using Ptr_t = std::unique_ptr<T>;
    /*! This class imitates that of the unique pointer so it exposes the same
     * member types*/
    using pointer = typename Ptr_t::pointer;
    using element_type = typename Ptr_t::element_type;
    using deleter_type = typename Ptr_t::deleter_type;
    //! The default constructor
    /*
     * The synthetic one is ok since the default constructor of a unique_ptr
     * sets it to the null pointer.
     */
    PointerWrapper() = default;


    /*!
     * This constructor takes a reference to an object of type T or derived  from
     * T. It defines the conversion T& -> Wrapper<T> and thus the conversion
     * from any reference to a class derived from T.
     * It uses clone() to clone the resource.
     * @note it implements the Prototype Pattern
     * @param resource The resource to be cloned into
     */
    PointerWrapper(const T &resource) : DataPtr(resource.clone()) {}

    //!
    /*!
     * A unique pointer to T is moved into the wrapper
     *
     * @param p The unique pointer to be moved into this class
     */
    PointerWrapper(Ptr_t &&p) noexcept : DataPtr(std::move(p)) {}

    /*!
     * Taking a pointer. Equivalent to  what unique_ptr does
     * @param p The pointer of type T*
     * @note Now the Wrapper has the ownership of the resource!
     */
    explicit PointerWrapper(T *p) noexcept : DataPtr(p) {}

    /*!
     * Copy constructor. Uses clone to clone the resource
     * @param original
     */
    PointerWrapper(const PointerWrapper<T> &original)
    : DataPtr(original.get() ? original.DataPtr->clone() : Ptr_t{})
    {}

    //! Copy conversion
    /*!
     * This constructor takes any PointerWrapper<U> with U equal to T or a type
     * derived from T. It allows the conversion from Wrapper<Derived> to
     * Wrapper<Base>.
     * @tparam U the type of the origin Wrapper, must be T or derived from T
     * @param original The original wrapper
     */
    template <class U>
    PointerWrapper(const PointerWrapper<U> &original)
    {
      if (original.get())
        {
          DataPtr.reset(static_cast<Ptr_t>(original.DataPtr->clone()));
        }
    }

    //! copy-assignement operator
    /*!
     * It resets current resource and clone that of the other wrapper
     */
    PointerWrapper &
    operator=(const PointerWrapper<T> &original)
    {
      if (this != &original)
        DataPtr = original.DataPtr ? original.DataPtr->clone() : Ptr_t{};
      return *this;
    }
    //! Maybe I want to mov-assign a unique pointer
    /*!
     * If argument is an rvalue unique_ptr can be moved.
     */
    PointerWrapper &
    operator=(Ptr_t &&p) noexcept
    {
      DataPtr = std::move(p);
      return *this;
    }
    //! Copy-Assignment of a unique pointer by cloning
    /*!
     *  If argument is an lvalue I need clone()
     */
    PointerWrapper &
    operator=(const Ptr_t &p)
    {
      DataPtr = p->clone(); // note that this is a move-assignment
      return *this;
    }

    //! The move constructor
    /*!
     * @param rhs the wrapper to be moved
     * @note unique_ptr can be moved
     */
    PointerWrapper(PointerWrapper<T> &&rhs) = default;
    /*!
     *  To allow conversion in move constructor
     * @param rhs the wrapper to be moved
     */
    template <class U>
    PointerWrapper(PointerWrapper<U> &&rhs) noexcept :
    DataPtr(static_cast<T *>(rhs.release()))
    {}

    //! Move assignement
    /*!
     * @param rhs the wrapper to be moved
     */
    PointerWrapper &operator=(PointerWrapper<T> &&rhs) = default;
    /*!
     * To allow for conversion Derived -> Base
     * @param rhs the wrapper to be moved, may be a wrapper to a derived type
     * @note maybe not required since I have conversion in the move constructor.
     * After the assignment the rhs is null.
     */
    template <class U>
    PointerWrapper &
    operator=(PointerWrapper<U> &&rhs) noexcept
    {
      using otherPType = typename PointerWrapper<U>::pointer;
      static_assert(std::is_convertible<otherPType, pointer>::value,
                    "Pointers must be convertible");
      if (this->get() != static_cast<pointer>(rhs.get()))
        {
          DataPtr.reset(rhs.get());
        }
      rhs.release(); // release resource
      return *this;
    };

    /*! Dereferencing operator. The PointerWrapper works like a pointer to T*/
    const T &
    operator*() const noexcept
    {
      return *DataPtr;
    }
    /*! Dereferencing operator. The PointerWrapper works like a pointer to T*/
    T &
    operator*() noexcept
    {
      return *DataPtr;
    }

    /*! Dereferencing operator. The PointerWrapper works like a pointer to T*/
    const T *
    operator->() const noexcept
    {
      return DataPtr.get();
    }
    /*! Dereferencing operator. The PointerWrapper works like a pointer to T*/
    T *
    operator->() noexcept
    {
      return DataPtr.get();
    }
    //! It releases the resource returning a pointer
    auto
    release() noexcept
    {
      return DataPtr.release();
    }
    /*! Deletes the resource
     *  You can pass the pointer of a new resource to hold
     *  @param ptr the pointer to the new resource, defaulted to nullprt
     */
    void
    reset(pointer ptr = nullptr) noexcept
    {
      DataPtr.reset(ptr);
    }
    /*! swap wrappers */
    void
    swap(PointerWrapper<T> &other) noexcept
    {
      DataPtr.swap(other.DataPtr);
    }
    /*! get the pointer */
    pointer
    get() const noexcept
    {
      return DataPtr.get();
    }
    /*!
     * @return The deleter object which would be used for destruction of the managed object.
     */
    auto & get_deleter() noexcept
    {
      return DataPtr.get_deleter();
    }
    /*!
     * @return The deleter object which would be used for destruction of the managed object.
     */
    auto const & get_deleter() const noexcept
    {
      return DataPtr.get_deleter();
    }

    /*! conversion to bool */
    explicit operator bool() const noexcept { return static_cast<bool>(DataPtr); }

  private:
    Ptr_t DataPtr;
  };
  //! Utility to make a PointerWrapper
  /*!
  Creates a PointerWrapper<Base> indicating Base and Derived class.
  I need two compulsory template parameters, one for the Base
  and one for the concrete Derived class since I need to construct
  a derived object. Of  course,

  @note I may have B=D.

  @tparam B A base class. The function returns PointerWrapper<B>
  @tparam D The derived class. The wrapper will own an object of type D.
  @tparam Args Automatically deduced possible arguments for the constructor of D
  @param args The (possible) arguments of type Args
  @return A PointerWrapper<B>
   */
  template <class B, class D, typename... Args>
  PointerWrapper<B>
  make_PointerWrapper(Args &&... args)
  {
    return PointerWrapper<B>(std::make_unique<D>(std::forward<Args>(args)...));
  }
  /*!
  Creates a PointerWrapper the class to hold.
  @tparam D The PointerWrapper will own an object of type D.
  @tparam Args Automatically deduced possible arguments for the constructor of D
  @param args The (possible) arguments of type Args
  @return A PointerWrapper<D>
   */
  //template <class D, typename... Args>
  //PointerWrapper<D>
  //make_PointerWrapper(Args &&... args)
  //{
  //  return PointerWrapper<D>{std::make_unique<D>(std::forward<Args>(args)...)};
  //}
  //! comparison operator
  template <class T, class U>
  bool
  operator<(PointerWrapper<T> const &a, PointerWrapper<U> const &b)
  {
    return a.get() < b.get();
  }
  //! comparison operator
  template <class T, class U>
  bool
  operator<=(PointerWrapper<T> const &a, PointerWrapper<U> const &b)
  {
    return a.get() <= b.get();
  }
  //! comparison operator
  template <class T, class U>
  bool
  operator>(PointerWrapper<T> const &a, PointerWrapper<U> const &b)
  {
    return a.get() > b.get();
  }
  //! comparison operator
  template <class T, class U>
  bool
  operator>=(PointerWrapper<T> const &a, PointerWrapper<U> const &b)
  {
    return a.get() >= b.get();
  }
  //! comparison operator
  template <class T, class U>
  bool
  operator==(PointerWrapper<T> const &a, PointerWrapper<U> const &b)
  {
    return a.get() == b.get();
  }
  //! comparison operator
  template <class T, class U>
  bool
  operator!=(PointerWrapper<T> const &a, PointerWrapper<U> const &b)
  {
    return a.get() != b.get();
  }
} // end namespace apsc
/*!
 *  Specialization of std::hash. I can store wrappers in unordered associative
 * containers
 *  @note I rely on the fact the the standard library provides the hash for any
 * pointer type
 */
template <class T> struct std::hash<class apsc::PointerWrapper<T> >
{
  std::size_t
  operator()(const apsc::PointerWrapper<T> &w) const noexcept
  {
    return std::hash<typename apsc::PointerWrapper<T>::pointer>{}(w.get());
  }
};

#endif
/*
 *
 * Copyright (c) 2020
 * Luca Formaggia
 *
 * Permission to use, copy, modify, distribute and sell this software for
 * any purpose is hereby granted without fee, provided that the above
 * copyright notice appear in all copies and that both that copyright
 * notice and this permission notice appear in supporting documentation.
 * The authors make no representations about
 * the suitability of this
 * software for any purpose. It is provided "as is" without express or
 * implied warranty.
 */
