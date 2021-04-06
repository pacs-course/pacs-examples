#ifndef HH_INEQUALITY_HH
#define HH_INEQUALITY_HH
// Another interesting examples of use of CRTP (from Discovering Modern C++: An intensive Course...)
// I want to be able to define directly the != operator for any class that defines the == operator.
// I create a base template class that just do that by delegating the == operator to the derived class
template <typename T>
struct inequality
{
  bool operator !=(const T& that) const
  {
    return !(static_cast<const T&>(*this) == that);
  }
};




// See point.hpp for a possible use


#endif
