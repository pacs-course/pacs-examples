#ifndef HH_POINT_HH
#define HH_POINT_HH
// I want inequality
#include "inequality.hpp"
#include <array>
//! I use CRTP to have all relational operator consisten with the less operator
//!
/*!  Note that this is not strictly necessary here since array has the relational
 *  operators. But it is a nice simple example of a possible use of CRTP.
 */
namespace apsc::crtp
{
template <unsigned int N> class Point : public RelationalOps<Point<N> >
{
public:
  Point(std::array<double, N> const &in) : coord(in){};
  // default constructor
  Point() = default;
  double &
  operator[](unsigned int i)
  {
    return coord[i];
  }
  double
  operator[](unsigned int i) const
  {
    return coord[i];
  }
  //! less operator
  bool
  operator<(Point<N> const &that) const
  {
    unsigned int k{0u};
    for(auto const &i : coord)
      {
        if(i > that[k])
          return false;
        if(i < that[k])
          return true;
      }
    return false;
  }

private:
  std::array<double, N> coord;
};
}
#endif
