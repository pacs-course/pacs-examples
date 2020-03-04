// An example of use of decltype and decleval (and also of some type traits...
#include <iostream>
#include <type_traits> // for is_constructible
#include <utility> // for declval
#include <typeinfo> // for typeid
//! A class without default constructor
class NotDefaultConstructible
{
public:
  NotDefaultConstructible(int i):i(i){}
  int get_i() const {return this->i;}
  static const int j{10};
private:
  int i;
};
//! Another class 
class DefaultConstructible
{
public:
  DefaultConstructible()=default;
  DefaultConstructible(double i):i(i){}
  double get_i() const {return this->i;}
  void set_i(double x){this->i=x;};
  // A non integral static member cannot be initialised in class, not even in c++14, unless constexpr or
  // or (since c++17) inline
  inline static const double j=40.0;
private:
  double i=10.0;
};
// Before c++17  non integral static member could not be initialised in class, unless constexpr!
// You  have to do it out of class (in general in a cpp file):
// const double DefaultConstructible::j=40.0;

//! A template function where I want to interrogate the type of get_i().
template<class T>
void fun(T const & a)
{
  std::cout<<std::boolalpha;
  std::cout<<" The argument is:"<<std::endl;
  std::cout<<" Default constructible?     "<<std::is_default_constructible<T>::value<<std::endl;
  std::cout<<" Constructible from an int? "<<std::is_constructible<T, int>::value<<std::endl;
  // To interrogate the return get I do not need to create an object of type T!
  decltype(std::declval<T>().get_i()) b;
  // Which tye is b?, now to write it out I need typeid
  std::cout<<"get_i() returns a "<<typeid(b).name()<<std::endl;
  // I do not need declval to interrogate static members since I do not need
  // to create an object 
  std::cout<<"Static member j is of type "<<
    typeid(decltype(T::j)).name()<<std::endl;
}

int main()
{
  NotDefaultConstructible nd{5};
  DefaultConstructible d;
  std::cout<<"calling fun(NotDefaultConstructible) "<<std::endl;
  fun(nd);
  std::cout<<std::endl<<"calling fun(DefaultConstructible) "<<std::endl;
  fun(d);
  
}
