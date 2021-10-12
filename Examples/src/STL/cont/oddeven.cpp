#include <algorithm>
#include <iostream>
#include <iterator>
#include <set>
using namespace std;
//! Comparison operator
/*!
  Even numbers are "smaller" than odd numbers
 */
struct OddEven
{
  //! must be either const or static
  bool
  operator()(int const &i, int const &j) const
  {
    return i % 2 < j % 2;
  }
};

int
main()
{
  multiset<int, OddEven> ms = {10, 9, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  //! A set with the same element and same ordering
  set<int, OddEven> s{10, 9, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  // print all elements in the multiset
  for(int elem : ms)
    {
      cout << elem << ' ';
    }
  cout << endl;

  // print all elements in the set
  for(int elem : s)
    {
      cout << elem << ' ';
    }
  cout << endl;

  auto p = ms.equal_range(1);
  // print all elements equivalent to 1
  copy(p.first, p.second, ostream_iterator<int>(cout, " "));
  cout << endl;
}
