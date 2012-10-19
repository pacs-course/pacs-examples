
// using traits

#include <iostream>
#include <complex>
#include <algorithm>

template<class T> struct RTrait {
  typedef T RType;
};

template<class T> 
struct RTrait< std::complex<T> > {
  typedef T RType;
};

template<class T> 
typename RTrait<T>::RType maxnorm(T u[], int n) {
  typename RTrait<T>::RType norm = 0;
  for (int i = 0; i < n; i++) norm = std::max(norm, std::abs(u[i]));
  return norm;
}

int main() {

  int u[] = { 1, 2, 3, 4};

  std::complex<double> v[4];
  for (int i = 0; i < 4; i++) {
    //std::complex<double> a(i,i+1);
    //v[i] = a;
    v[i] = std::complex<double>(i,i+1);
  }
  for (int i = 0; i < 4; i++) std::cout << v[i] << '\n';

  std::cout << maxnorm(u, 4) << '\n';
  std::cout << maxnorm(v, 4) << '\n';

} // end main()
