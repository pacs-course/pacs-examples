#ifndef HH_API_HH
#define HH_API_HH
#include <memory>
#include "implementation.hpp"
namespace API
{
  //! this inteface exposes only the method get_a
  class user_interface
  {
  public:
    //! for simplicity I use in class definition
    double get_a() const {return pimpl->get_a();}
    //! non const version beware since pimpl is
    //! a pointer to a non-const implementation
    //! pimpl->get_a() is always the non const version
    double & get_a(){return pimpl->get_a();}
  private:
    std::unique_ptr<internal::implementation>
    pimpl{new internal::implementation};
  };
}// end API
#endif
