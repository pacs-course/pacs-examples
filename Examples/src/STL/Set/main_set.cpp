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
  //! Must be a non-static const member! 
  bool operator()(int const & a, int const & b) const
  {return a>b;}
};

int main()
{
  using namespace std;
  // I create my comparison operator using a lambda
  auto comp=[](int const & a,int const & b){return a>b;};
  // I need to give the type of the comparison operator, so I have to use decltype
  // The comparison operator has to be passed also argument to the constructor
  // since it is a lambda and a lambda has no default constructor
  set<int,decltype(comp)> s{comp};
  
  s.insert(10);
  s.insert(11);
  for (auto i: s) cout<<i<<" ";
  cout<<endl;
  // Another possibility is to wrap the lambda in a function wrapper
  std::function<bool (int const&, int const &)> compfun=comp;
  //Initialization via initializer list
  // I have give the comparison object explicitly. The rule is if the comparison object has a state
  // you have to pass it through the constructor, since we need to pass its state. Otherwise,
  // the set builds one with the default constructor.
  set<int,decltype(compfun)> t{{1,2,3,4,5,6,7},compfun};
  // But in this case you can also simply do (compfun is stateless and std::function is default-constructible
  //  set<int,decltype(compfun)> t{{1,2,3,4,5,6,7}};

  // or use this alternative syntax.
  // set<int,decltype(compfun)> t={{1,2,3,4,5,6,7},compfun};

  for (auto i: t) cout<<i<<" ";
  cout<<endl;
  // using the functor (no need to give the functor explicitely as argument)
  // I use initialization via parameter list 
  set<int,comparison> other={5,6,7,8,9,10,11};
  for (auto i: other) cout<<i<<" ";
  cout<<endl;
}
