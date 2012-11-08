#ifndef DISTANCE_HPP
#define DISTANCE_HPP

template <typename ObjectT>
struct EuclideanDistance
{
  real operator()( ObjectT const & p1, ObjectT const & p2 )
  {
    return ( p1 - p2 ).norm();
  }
};

#endif // DISTANCE_HPP
