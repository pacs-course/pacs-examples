#include <iostream>
#include <vector>
#include <set>
#include <cmath>
#include <numeric>
#include <limits>
#include <algorithm> //for shuffle
#include <random> // for random numbers
// Compile with -DREAL=float if you want floats
// or with -D"long double" if you want extended precision
#ifndef REAL
#define REAL  double
#endif
// Compile with -DREAL=float if you want floats
int main()
{
  // Set to the maximum precition for the chosen float type
  std::cout.precision(std::numeric_limits<REAL>::digits10+1);
  std::vector<REAL> a;
  // create a vector that sums to  a value
  REAL constexpr ExactValue=2.0;
  std::cout<<"Exact value: "<<ExactValue<<std::endl;
  // The number of elements in the vector is 2*N+1
  constexpr unsigned int N=100000;
  for (unsigned int i=0u; i<=N;++i)
    {
      auto x=std::pow(static_cast<REAL>(i),3);
      a.push_back(x);
      a.push_back(-x);
    }
  a.push_back(ExactValue);
  // min e max_element return an iterator to the found element, so I have to dereference it.
  std::cout<<"Max and Min values"<<*(std::min_element(a.begin(),a.end()))<<", "<<
    *(std::max_element(a.begin(),a.end()))<<std::endl;
  // Now shuffle. 
  // Create a random seed using the random_device. It is different everytime!
  std::random_device rd;
  // A random number generator, initialised with a random seed
  std::mt19937 g(rd());
  std::shuffle(a.begin(),a.end(),g);
  // an example of accumulate: computes the sum of the range, the last argument is
  // the initial value (it could have been omitted since it is defualted to zero)
  auto sum = std::accumulate(a.begin(),a.end(),0.0);
  // but you can do also simply
  // REAL sum=0;
  // for (auto s : a) sum+=s;
  std::cout<<"Normal sum:"<<sum<<" Error (%):"<<100*(ExactValue-sum)/ExactValue<<std::endl;
  // A trick. Order the data so that smallest (in absolute value)
  // are added first, separating positive and negative values
  std::vector<REAL> p; // positive values
  std::vector<REAL> n; // non-positive values
  for (auto s : a)
    {
      if (s>0)p.push_back(s);
      else n.push_back(s);
    };
  // Order from smallest to largest in abs value
  std::sort(p.begin(),p.end());// std::smallest is the defualt
  // To order negative values from smallest in abs value I sort using std::greater
  std::sort(n.begin(),n.end(),std::greater<REAL>());
  auto res=std::accumulate(p.begin(),p.end(),0.0)+ std::accumulate(n.begin(),n.end(),0.0);
  std::cout<<"Ordered summation:"<<res<<" Error (%):"<<100*(ExactValue-res)/ExactValue<<std::endl;
  
}
