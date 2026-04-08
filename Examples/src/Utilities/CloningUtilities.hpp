#ifndef CLONINGANDPOINTERWRAPPER_H
#define CLONINGANDPOINTERWRAPPER_H
#include <compare>
#include <concepts>
#include <functional>
#include <memory>
#include <type_traits>
#include <utility>
namespace apsc
{
/*
 * @note Utilities in this header rely on apsc::TypeTraits.
 */
namespace TypeTraits
{
  /*!
   * @brief Type trait checking whether a type provides a valid clone().
   *
   * The primary template maps to false.
   * @tparam T The class type to be checked
   * @tparam Sfinae SFINAE parameter used by specializations
   */
  template <typename T, typename Sfinae = void>
  struct has_clone : public std::false_type
  {};

  /*!
  @brief Specialization enabled when a const object provides clone().

  This specialization is true only if the expression const T&.clone() exists
  and std::unique_ptr<T> is convertible to the clone() return type.

  @tparam T The class type to be checked
   */
  template <typename T>
  struct has_clone<T, std::void_t<decltype(std::declval<const T &>().clone())>>
    : public std::bool_constant<std::is_convertible_v<
        std::unique_ptr<T>, decltype(std::declval<const T &>().clone())>>
  {};

  //! A helper function
  /*!
   * @brief Returns true if T satisfies has_clone.
   * @tparam T Type to test
   */
  template <class T>
  constexpr bool
  isClonable()
  {
    return has_clone<T>::value;
  }

  /*!
   * @brief Convenience variable template exposing has_clone<T>::value.
   * @tparam T Type to test
   */
  template <typename T> constexpr bool has_clone_v = has_clone<T>::value;

  /*!
   * @brief Concept matching types with a valid clone().
   */
  template <class T>
  concept Clonable = has_clone_v<T>;

} // end namespace TypeTraits

/*! A smart pointer that handles cloning for compusing with polymorphic objects

This class implements a generic wrapper around a unique pointer
useful to support the bridge pattern. Its role is to ease the memory
management of object which are composed polymorphically in a class
in order to implement a rule.  It is a extensive modification of the
class presented by Mark Joshi in his book "c++ design patterns and
derivative pricing", Cambridge Press. This version makes use of
std::unique_ptr to ease the handling of memory.

It handles memory as a unique_ptr, but implements copy operations by
cloning the resource.

We make an example to illustrate the problem. Let's consider first a
solution with aggregation

@code

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
@endcode

The problem here is that the scope of an object of type UseB and
that of the object of the base class B (or possibly reference)
passed to the constructor are linked. For instance the statements

@code
B* a = new D1;
UseB foo(a)
delete a;
@endcode

would be invalid because the copy of a stored in foo is now
dangling. A similar situation happens with references.  Moreover,
the state of UseB depends on the state of a! And that is not what
we want here, we want the object pointed by Impl to be part of
class UseB: we want composition not aggregation!

But how to compose a polymorphic object? A possibility is to add
to B and to D1 the method clone()

@code
virtual std::unique_ptr<B> clone() const {return std::make_unique<D>(*this);}
@endcode

Then, if useB::Impl is now a unique_ptr<B>, the constructor of useB may do
@code
useB(const B&):Impl(B.clone()){}
@endcode

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

@code
class UseB{
public:
UseB(const B &):Impl(B); The constructor takes a reference to B
....
methods that use B* polymorphically
....
private:
// I use the wrapper that behaves like a pointer!
PointerWrapper<B> Impl;
@endcode


@pre The parameter class T must expose clone() on const objects,
and clone() must return a type convertible to std::unique_ptr<T>.

@tparam T the base class
  */
template <TypeTraits::Clonable T> class PointerWrapper
{
public:
  // Check if clone is present
  // Here I prefer not using concepts to have a more extensive error message
  // I am using concepts now
  /*static_assert(
    TypeTraits::isClonable<T>(),
    "template parameter of Wrapper must be a clonable class. "
    "You must have a virtual clone()const method that returns a unique_ptr<T>");
    */
  // The type of the stored unique pointer
  using Ptr_t = std::unique_ptr<T>;
  /*! This class imitates that of the unique pointer so it exposes the same
   * member types*/
  using pointer = typename Ptr_t::pointer;
  using element_type = typename Ptr_t::element_type;
  using deleter_type = typename Ptr_t::deleter_type;
  //! The default constructor
  /*
   * Creates an empty (null) PointerWrapper.
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
    : DataPtr{original.get() ? original.DataPtr->clone() : Ptr_t{}}
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
    requires std::is_convertible_v<U *, T *>
  PointerWrapper(const PointerWrapper<U> &original)
  {
    if(original.get())
      {
        DataPtr = original.get()->clone();
      }
  }

  //! copy-assignement operator
  /*!
   * It resets current resource and clone that of the other wrapper
   */
  PointerWrapper &
  operator=(const PointerWrapper<T> &original)
  {
    if(this != &original)
      DataPtr = original.DataPtr ? original.DataPtr->clone() : Ptr_t{};
    return *this;
  }
  /*!
   * @brief copying assignment allowing for conversions
   *
   * This assignment allows converting PointerWrapper<Derived> to
   * PointerWrapper<Base>. If U is not convertible to T, the requires-clause
   * rejects this overload.
   * @tparam U The derived type
   * @param original The Wrapper to convert-copy
   * @return a reference to myself
   */
  template <class U>
    requires std::is_convertible_v<U *, T *>
  PointerWrapper &
  operator=(const PointerWrapper<U> &original)
  {
    DataPtr = original.get() ? original.get()->clone() : Ptr_t{};
    return *this;
  }

  //! Maybe I want to move-assign a unique pointer
  /*!
   * If argument is an rvalue unique_ptr it can be moved.
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
    DataPtr = p ? p->clone() : Ptr_t{}; // note that this is a move-assignment
    return *this;
  }

  //! The move constructor
  /*!
   * @param rhs the wrapper to be moved
   * @note unique_ptr can be moved
   */
  PointerWrapper(PointerWrapper<T> &&rhs) = default;
  /*!
   * @brief Move conversion constructor.
   *
   * Enabled only when U* is convertible to T*.
   * @param rhs the wrapper to be moved
   */
  template <class U>
    requires std::is_convertible_v<U *, T *>
  PointerWrapper(PointerWrapper<U> &&rhs) noexcept : DataPtr{rhs.release()}
  {}

  //! Move assignement
  /*!
   * @param rhs the wrapper to be moved
   */
  PointerWrapper &operator=(PointerWrapper<T> &&rhs) = default;
  /*!
   * @brief Move assignment allowing conversion Derived -> Base.
   *
   * Enabled only when U* is convertible to T*.
   * @param rhs the wrapper to be moved, may be a wrapper to a derived type
   * @note After assignment rhs is null.
   */
  template <class U>
    requires std::is_convertible_v<U *, T *>
  PointerWrapper &
  operator=(PointerWrapper<U> &&rhs) noexcept
  {
    if(this->get() != static_cast<pointer>(rhs.get()))
      {
        DataPtr.reset(rhs.release());
      }
    return *this;
  }

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
  pointer
  release() noexcept
  {
    return DataPtr.release();
  }
  /*! Deletes the resource
   *  You can pass the pointer of a new resource to hold
   *  @param ptr the pointer to the new resource, defaulted to nullptr
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
   * @return The deleter object which would be used for destruction of the
   * managed object.
   */
  deleter_type &
  get_deleter() noexcept
  {
    return DataPtr.get_deleter();
  }
  /*!
   * @return The deleter object which would be used for destruction of the
   * managed object.
   */
  deleter_type const &
  get_deleter() const noexcept
  {
    return DataPtr.get_deleter();
  }

  /*! conversion to bool */
  explicit
  operator bool() const noexcept
  {
    return static_cast<bool>(DataPtr);
  }

private:
  Ptr_t DataPtr;
};
//! Utility to make a PointerWrapper
/*!
@brief Creates a PointerWrapper<Base> owning a Derived object.

This overload takes both Base and Derived template parameters.

@note B and D may be the same type.
@note This overload is constrained with std::derived_from and
std::constructible_from.

@tparam B Base class type of the wrapper
@tparam D Concrete class to instantiate and own
@tparam Args Constructor argument types for D
@param args Arguments forwarded to D's constructor
@return A PointerWrapper<B>
 */
template <class B, class D, typename... Args>
  requires std::derived_from<D, B> && std::constructible_from<D, Args...>
PointerWrapper<B>
make_PointerWrapper(Args &&...args)
{
  return PointerWrapper<B>{std::make_unique<D>(std::forward<Args>(args)...)};
}
/*!
@brief Creates a PointerWrapper with inferred base type.
@tparam D The PointerWrapper will own an object of type D.
@tparam Args Automatically deduced constructor argument types for D.
@param args Arguments forwarded to D's constructor.
@return A PointerWrapper<D>
 */
// template <class D, typename... Args>
// PointerWrapper<D>
// make_PointerWrapper(Args &&... args)
//{
//  return PointerWrapper<D>{std::make_unique<D>(std::forward<Args>(args)...)};
//}
//! comparison operator
#if __cplusplus < 202002L
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
#else
/* C++20 use the spaceship operator to simplify things */
template <class T, class U>
auto // you need auto, let the compiler to the stuff
operator<=>(PointerWrapper<T> const &a, PointerWrapper<U> const &b) noexcept
{
  return a.get() <=> b.get();
}
template <class T, class U>
//! Equivalence operator. I need it since it cannot be deduced by <=>
bool
operator==(PointerWrapper<T> const &a, PointerWrapper<U> const &b) noexcept
{
  return a.get() == b.get();
}
/*!
@brief Three-way comparison with nullptr.
@details This overload mirrors the corresponding std::unique_ptr comparison.
The requires-clause ensures the underlying pointer type is three-way
comparable with std::nullptr_t.
@code
template< class T>
auto
operator<=>( const PointerWrapper<T>& x, std::nullptr_t )
{
    return x.get() <=> nullptr;
};
@endcode
@note This overload is required because it is not deduced from wrapper-wrapper
comparison.
@tparam T The type of the pointer
@param x The wrapper
@param nullptr Null pointer literal used in the comparison
@return Result of comparing x.get() with nullptr

*/
template <class T>
  requires std::three_way_comparable<typename PointerWrapper<T>::pointer>
std::compare_three_way_result_t<typename PointerWrapper<T>::pointer>
operator<=>(const PointerWrapper<T> &x, std::nullptr_t) noexcept
{
  return x.get() <=> nullptr;
};
template <class T>
bool
operator==(const PointerWrapper<T> &x, std::nullptr_t) noexcept
{
  return x.get() == nullptr;
};

#endif

} // end namespace apsc
/*!
 *  Specialization of std::hash. I can store wrappers in unordered associative
 * containers
 *  @note I rely on the fact the the standard library provides the hash for any
 * pointer type
 */
namespace std
{
template <class T> struct hash<apsc::PointerWrapper<T>>
{
  std::size_t
  operator()(const apsc::PointerWrapper<T> &w) const noexcept
  {
    return std::hash<typename apsc::PointerWrapper<T>::pointer>{}(w.get());
  }
};
} // namespace std

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
