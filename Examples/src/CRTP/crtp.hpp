#ifndef __HHH_CRTP_HH__
#define __HHH_CRTP_HH__
//! @file crtp.hpp
//! @brief An example of Curiusly Recurring Template Patterns

//! Base class
template <typename T> class Base
{
public:
  //! Delegates to derived class
  double
  fun(double const &x) const
  {
    return derived().fun(x);
  }
  //! Example with a static function
  static constexpr int
  fstatic()
  {
    return T::fstatic();
  }

private:
  //! Returns myself as derived
  T &
  derived()
  {
    return static_cast<T &>(*this);
  }
  //! Const version
  T const &
  derived() const
  {
    return static_cast<T const &>(*this);
  }
};

//! Derived class
class Derived1 : public Base<Derived1>
{
public:
  //! Implementation of function fun
  double fun(double const &x) const;
  //! Implementation of the static function.
  static constexpr int
  fstatic()
  {
    return N;
  }

private:
  static constexpr int N = 1;
};
//! Derived class
class Derived2 : public Base<Derived2>
{
public:
  //! Implementation of function fun
  double fun(double const &x) const;
  //! Implementation of the static function.
  static constexpr int
  fstatic()
  {
    return N;
  }

private:
  static constexpr int N = 2;
};

#endif
