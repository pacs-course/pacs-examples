#ifndef HH_INEQUALITY_HH
#define HH_INEQUALITY_HH
// Another interesting examples of use of CRTP (from Discovering Modern C++: An
// intensive Course...) I want to be able to define directly the relational operator for
// any class that defines the < operator. I create a base template class that
// just do that by delegating the == operator to the derived class
namespace apsc
{
template <typename D> struct RelationalOps
{
  bool
  operator==(const D &that) const
  {
    return not(asD() < that) and not(that < asD());
  }
  bool
  operator<=(const D &that) const
  {
    return (asD() < that) or (that == asD());
  }
  bool
  operator>(const D &that) const
  {
    return not(asD() < that) and not(that == asD());
  }
  bool
  operator>=(const D &that) const
  {
    return not(asD() < that);
  }
  bool
  operator!=(const D &that) const
  {
    return not(asD() == that);
  }

private:
  /*!
   * @fn const D asD&()const
   * @brief returns myself as a D object
   * Clearly I must be a base class for D for this to work
   * @return Myself as a reference (const) to D
   */
  D const &
  asD() const
  {
    return static_cast<const D &>(*this); // downcast!
  }
};
}
// See point.hpp for a possible use

#endif
