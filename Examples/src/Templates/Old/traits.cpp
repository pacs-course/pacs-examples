
// using traits

#include <iostream>
#include <complex>
#include <algorithm>
#include <numeric>
#include <vector>
using std::vector;

//  Use of a trait to specify the return value of a function

// Base template
template<class T> struct RTrait {
  typedef T RType;
};

//If I use complex

template<class T> 
struct RTrait< std::complex<T> > {
  typedef T RType;
};



template<class T> 
typename RTrait<T>::RType maxnorm(vector<T> & u) {
  typename RTrait<T>::RType norm = 0;
  typedef typename vector<T>::size_type size_t;
  for (size_t i = 0; i < u.size(); i++) norm = std::max(norm, std::abs(u[i]));
  return norm;
}

// A different implementation using accumulate
// I create a functor for max 
template <class T, template<class > class Trait=RTrait>
class myMax
{
public:
  typedef typename Trait<T>::RType RType;
  RType operator() (RType const & a, T const & b)
  {
    return std::max(a, std::abs(b));
  }
};




int main() {
  using std::vector;
  using std::cout;
  using std::endl;
  using std::complex;

  
  vector<int> u;
  
  int ud[] = { 1, 2, 3, 4};
  u.insert(u.begin(),ud,ud+4);
  
  vector<complex<double> > v(4);
  for (int i = 0; i < 4; i++) {
    v[i] = complex<double>(i,i+1);
  }
  for (int i = 0; i < 4; i++) std::cout << v[i] << endl;

  cout <<"Max norm of u "<< maxnorm(u) << endl;
  cout <<"Max norm of v "<< maxnorm(v) << endl;

  // Using accumulate(Initer, LastIter,InitValue,Operator)
  cout <<"Max norm of u "<< std::accumulate(u.begin(),u.end(),0.0,myMax<double>()) << endl;
  cout <<"Max norm of v "<< std::accumulate(v.begin(),v.end(),0.0,myMax<complex<double> >())<< endl;
  

} // end main()
