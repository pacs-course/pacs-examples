//! Other interesting use of tie
/*!
  Taken from cpprefernce.com
 */
#include <iostream>
#include <string>
#include <set>
#include <tuple>

struct S {
  int n;
  std::string s;
  float d;
  //! less-than comparison implemented as a method
  /*! 
    You can use tie to force lexicografic comparison in a simple way!
  */
  bool operator<(const S& rhs) const
  {
    // compares n to rhs.n,
    // then s to rhs.s,
    // then d to rhs.d
    return std::tie(n, s, d) < std::tie(rhs.n, rhs.s, rhs.d);
  }
   bool operator==(const S& rhs) const
  {
    // compares n to rhs.n,
    // then s to rhs.s,
    // then d to rhs.d
    return std::tie(n, s, d) == std::tie(rhs.n, rhs.s, rhs.d);
  }
};

int main()
{
  std::set<S> set_of_s; // S is LessThanComparable!
  
  S value{42, "Test", 3.14};
  bool inserted;
  
  // unpacks the return value of insert into iter and inserted
  // tie works also with std::pairs!
  // I use std::ignore since the first term in the pair is
  // not used
  std::tie(std::ignore, inserted) = set_of_s.insert(value);
  
  if (inserted)
    std::cout << "Value was inserted successfully\n";
  else
            std::cout << "Value was not inserted\n";

  // But since C++17 things are even nicer using initilized if
  // and structured  bindings!
  // you will have a warning since it is not used. But with
  // structured bindings you cannot use std::ignore.
  
  if(auto [it,ins]=set_of_s.insert(value);ins)
        std::cout << "Value was inserted successfully\n";
  else
            std::cout << "Value was not inserted\n";


}
