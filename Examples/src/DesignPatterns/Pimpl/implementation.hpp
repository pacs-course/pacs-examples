#ifndef HH_IMPLEMENTATION_HH
#define HH_IMPLEMENTATION_HH
#include <array>
namespace internal
{
  //! A simple class that encupsulates the implementation
  //!
  //! This is an example. A real class has more complex stuff;
  //! Definitions in a source file since the pimpl idiom normally
  //! is implemented with a library
  class implementation
  {
  public:
    double get_a() const;
    int get_b() const;
    std::array<double,2> get_A() const;
    // The non const version is a setter
    // if is put on the left hand side
    double & get_a();
    int & get_b();
    std::array<double,2> & get_A();
  private:
    double M_a{0.0};
    int M_b{0};
    std::array<double,2> A{{0.0,0.0}};
  };
}// end internal
#endif
