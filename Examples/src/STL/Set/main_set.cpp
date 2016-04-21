//! An example on the use of set
/*!
  It gives and idea on how we can change the comparison operator for a set.
  And some example on parameter list initialization and use of decltype.
 */
#include<set>
#include<iostream>
#include<functional>
//! A function class
/*!
  We recall that a comparison operator has the signature
  \code{.cpp}
  bool ( T const &a, T const &b)
  \endcode
  and MUST satisfy:

\li a< b && a>b == false
\li !(a<b) && !(b<a) -> a==b

 */
struct comparison
{
  bool operator()(int const & a, int const & b)
  {return a>b;}
};

int main()
{
  using namespace std;
  // I create my comparison operator using a lambda
  auto comp=[](int const & a,int const & b){return a>b;};
  // I have to pass it explicitly since lambdas has NO DEFAULT CONSTRUCTOR
  set<int,decltype(comp)> s(comp);
  s.insert(10);
  s.insert(11);
  for (auto i: s) cout<<i<<" ";
  cout<<endl;
  // Another possibility is to wrap the lambda in a function
  std::function<bool (int const&, int const &)> compfun=comp;
  //Initialization via initializer list
  set<int,decltype(compfun)> t({1,2,3,4,5,6,7,},compfun);
  for (auto i: t) cout<<i<<" ";
  cout<<endl;
  // using the functor (no need to give the functor explicitely as argument)
  // I use initialization via parameter list 
  set<int,comparison> other={5,6,7,8,9,10,11};
  for (auto i: other) cout<<i<<" ";
  cout<<endl;
}
