//! An example on the use of set
/*!
  It gives and idea on how we can change the comparison operator for a set.
  And some example on parameter list initialization and use of decltype.
 */
#include <functional>
#include <iostream>
#include <set>
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
  bool
  operator()(int const &a, int const &b) const
  {
    return a > b;
  }
};

int
main()
{
  using namespace std;
  // I create my comparison operator using a lambda
  auto comp = [](int const &a, int const &b) { return a > b; };
  // I need to give the type of the comparison operator, so I have to use
  // decltype The comparison operator has to be passed also argument to the
  // constructor since it is a lambda and a lambda has no default constructor
  // @Note: this is just an example to show how to pass a lambda as comp operator.
  // Indeed it is much simpler in this case to use the standard functor greater<T>:
  // std::set<int,std::greater> s;

  set<int, decltype(comp)> s{comp};



  s.insert(10);
  s.insert(11);
  for(auto i : s)
    cout << i << " ";
  cout << endl;
  // Another possibility is to wrap the lambda in a function wrapper
  std::function<bool(int const &, int const &)> compfun = comp;
  // Initialization via initializer list
  // I have give the comparison object explicitly. The rule is if the comparison
  // object has a state you have to pass it through the constructor, since we
  // need to pass its state. Otherwise, the set builds one with the default
  // constructor.
  set<int, decltype(compfun)> t{{1, 2, 3, 4, 5, 6, 7}, compfun};
  // But in this case you can also simply do (compfun is stateless and
  // std::function is default-constructible
  //  set<int,decltype(compfun)> t{{1,2,3,4,5,6,7}};

  // or use this alternative syntax.
  // set<int,decltype(compfun)> t={{1,2,3,4,5,6,7},compfun};
  cout<<"t is\n";
  for(auto i : t)
    cout << i << " ";
  cout << endl;
  // using the functor (no need to give the functor explicitely as argument)
  // I use initialization via parameter list
  set<int, comparison> other = {5, 6, 7, 8, 9, 10, 11};
  for(auto i : other)
    cout << i << " ";
  cout << endl;

  // How to change a value. This is the correct way (since c+11)
  // Extract the node with the value to be changed
  auto node=t.extract(2); // extract the node containing 2
  if(!node.empty()) // 2 is present
    {
      node.value()=10; // change the node value
      t.insert(std::move(node));// move node back to the set
    }
  else // value not present in the set, Just add it
    {
      t.insert(10);
    }
  cout<<"t is now\n";
  for(auto i : t)
    cout << i << " ";
  cout << endl;



}
