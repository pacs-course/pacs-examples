#ifndef _PREPROC_HPP
#define _PREPROC_HPP
class Interval
{
public:
  Interval();
  Interval(double const & a, double const & b);
  double length();
  void getInterval(double  & left, double & right);
private:
  double myleft,myright;
  void swap();
};
#endif
  
