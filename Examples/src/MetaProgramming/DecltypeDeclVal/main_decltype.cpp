// An example of use of decltype and decleval (and also of some type traits...
#include <iostream>
#include <type_traits> // for is_constructible
#include <typeinfo>    // for typeid
#include <utility>     // for declval
//! A class without default constructor
class NotDefaultConstructible
{
public:
  NotDefaultConstructible(int i) : i(i) {}
  int
  get_i() const
  {
    return this->i;
  }
  int
  getTwiceJ(int k)
  {
    return j * k;
  }
  static const int j{10};

private:
  int i;
};
//! Another class
class DefaultConstructible
{
public:
  DefaultConstructible()
  {
    std::cout << "I am creating an object of type DefaultConstructible with "
                 "default constructor\n";
  };
  DefaultConstructible(double i) : i(i) {}
  double
  get_i() const
  {
    return this->i;
  }
  void
  set_i(double x)
  {
    this->i = x;
  };
  int
  getTwiceJ(int k)
  {
    return j * k;
  }
  // A non integral static member cannot be initialised in class, not even in
  // c++14, unless constexpr or or (since c++17) inline
  inline static const double j = 40.0;

private:
  double i = 10.0;
};
// You  have to do it out of class (before C++17):
// const double DefaultConstructible::j=40.0;

//! A template function where I want to interrogate the type of get_i().
template <class T>
void
fun(T const & /*a */) //  am not using the parameter, o I can omit indicating it
{
  std::cout << std::boolalpha;
  std::cout << " The argument is:" << std::endl;
  std::cout << " Default constructible?     "
            << std::is_default_constructible<T>::value << std::endl;
  std::cout << " Constructible from an int? "
            << std::is_constructible<T, int>::value << std::endl;
  if constexpr(std::is_default_constructible<T>::value)
    {
      // If I have a default constructor I can also do this
      decltype(T().getTwiceJ(double())) c;
      // Which type is c?, now to write it out I need typeid
      std::cout << "The type returned by getTwice(double) is  "
                << typeid(c).name() << std::endl;
    }
  else
    {
      // If I do not have a default constructor I can do this
      // (it works in any case, so THIS is the more correct construct!)
      decltype(std::declval<T>().getTwiceJ(double())) c;
      // Which type is c?, now to write it out I need typeid
      std::cout << "The type returned by getTwice(double) is  "
                << typeid(c).name() << std::endl;
    }
  // I do not need declval to interrogate static members since I do not need
  // to create an object: static members are attribute of the class.
  std::cout << "Static member j is of type " << typeid(decltype(T::j)).name()
            << std::endl;
}

int
main()
{
  NotDefaultConstructible nd{5};
  DefaultConstructible    d;
  std::cout << "calling fun(NotDefaultConstructible) " << std::endl;
  fun(nd);
  std::cout << std::endl << "calling fun(DefaultConstructible) " << std::endl;
  fun(d);
}
