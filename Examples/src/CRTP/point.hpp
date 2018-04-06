#ifndef HH_POINT_HH
#define HH_POINT_HH
// I want inequality
#include <array>
#include "inequality.hpp"
//! I use CRTP to have inequality if I define equality operator
/*!  Note that this is not strictly necessary here since array has the
inequality operator. But is a nice simple example.
 */
template<unsigned int N>
class Point : public inequality<Point<N>>
{
public:
  Point(std::array<double,N>const & in):coord(in){};
  //default constructor
  Point()=default;
  double & operator [] (unsigned int i){return coord[i];}
  double operator [] (unsigned int i)const {return coord[i];}
  //! operator defined in class for simplicity
  bool operator == (Point<N> const & that)const
  {
    unsigned int k{0u};
    for (auto const & i: coord)
      {
        if (i != that[k++]) return false;
      }
    return true;
  }
private:
  std::array<double,N> coord;
};
#endif
