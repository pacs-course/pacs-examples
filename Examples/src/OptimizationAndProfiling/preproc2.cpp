#include "preproc2.hpp"

Interval::Interval():myleft(0),myright(0)
{}

Interval::Interval(double const & a, double const & b):myleft(a),myright(b)
{ if(a> b) swap();}

double Interval::length()
{
  return myright-myleft;
}

void Interval::getInterval(double  & left, double & right)
{
  left=myleft;
  right=myright;
}

void Interval::swap()
{
  double tmp(myleft);
  myleft=myright;
  myright=tmp;
}
