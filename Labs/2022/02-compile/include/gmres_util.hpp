#ifndef HH_GMSER_UTIL_HH
#define HH_GMSER_UTIL_HH
#include <cmath>
#include <vector>
// using std::abs;

template <class Matrix, class Vector>
void
Update(Vector &                   x,
       int                        k,
       Matrix const &             h,
       Vector const &             s,
       std::vector<Vector> const &v)
{
  Vector y(s);

  // Backsolve:
  for (int i = k; i >= 0; i--)
    {
      y(i) /= h(i, i);
      for (int j = i - 1; j >= 0; j--)
        y(j) -= h(j, i) * y(i);
    }

  for (int j = 0; j <= k; j++)
    x += v[j] * y(j);
}


template <class Real>
void
GeneratePlaneRotation(Real &dx, Real &dy, Real &cs, Real &sn)
{
  if (dy == 0.0)
    {
      cs = 1.0;
      sn = 0.0;
    }
  else if (std::abs(dy) > std::abs(dx))
    {
      Real temp = dx / dy;
      sn        = 1.0 / sqrt(1.0 + temp * temp);
      cs        = temp * sn;
    }
  else
    {
      Real temp = dy / dx;
      cs        = 1.0 / sqrt(1.0 + temp * temp);
      sn        = temp * cs;
    }
}


template <class Real>
void
ApplyPlaneRotation(Real &dx, Real &dy, Real &cs, Real &sn)
{
  Real temp = cs * dx + sn * dy;
  dy        = -sn * dx + cs * dy;
  dx        = temp;
}

/*
template < class Real >
Real
abs(Real x)
{
  return (x > 0 ? x : -x);
}
*/


#endif
