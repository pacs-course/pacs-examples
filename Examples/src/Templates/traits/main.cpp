#include <iostream>
#include <cmath>
#include <complex>
//! \file magnitude.hpp
/*!
  A simple example of use of traits.

  We want to control how comparison is made
*/

//! The basic template

template <typename T>
struct Value_Trait
{
  using type=T;
};


template <class T>
struct Value_Trait<std::complex<T> >
{
    using type = T;
};


template <typename T>
T
inline min_magnitude(const T& x, const T& y)
{
    using std::abs;
    typename Value_Trait<T>::type ax= abs(x), ay= abs(y);
    return ax < ay ? x : y;
}


int main () 
{
    using namespace std;

    double               d1= 3., d2= 4.;
    cout << "min |d1, d2| = " << min_magnitude(d1, d2) << '\n';

    std::complex<double> c1(3.), c2(4.);
    cout << "min |c1, c2| = " << min_magnitude(c1, c2) << '\n';

    return 0 ;

}
