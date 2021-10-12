/* The following code example is taken from the book
 * "The C++ Standard Library - A Tutorial and Reference, 2nd Edition"
 * by Nicolai M. Josuttis, Addison-Wesley, 2012
 *
 * (C) Copyright Nicolai M. Josuttis 2012.
 * Permission to copy, use, modify, sell and distribute this software
 * is granted provided this copyright notice appears in all copies.
 * This software is provided "as is" without express or implied
 * warranty, and with no claim as to its suitability for any purpose.
 */
#include <algorithm>
#include <iostream>
#include <iterator>
#include <set>
#include <vector>
using namespace std;

int
main()
{
  // type of the collection:
  // - no duplicates
  // - elements are integral values
  // - descending order
  set<int, greater<int>> coll1;

  // insert elements in random order using different member functions
  coll1.insert({4, 8, 5, 1, 6, 2, 10, 13, 18});
  coll1.insert(5);

  // print all elements
  for(int elem : coll1)
    {
      cout << elem << ' ';
    }
  cout << endl;

  // insert 4 again and process return value
  auto status = coll1.insert(4);
  if(status.second)
    {
      cout << "4 inserted as element "
           << distance(coll1.begin(), status.first) + 1 << endl;
    }
  else
    {
      cout << "4 already exists" << endl;
    }

  // assign elements to another set with ascending order
  set<int> coll2(coll1.cbegin(), coll1.cend());

  // print all elements of the copy using stream iterators
  copy(coll2.cbegin(), coll2.cend(), ostream_iterator<int>(cout, " "));
  cout << endl;

  // remove all elements up to element with value 4
  coll2.erase(coll2.begin(), coll2.find(4));
  cout << " Removed all elements up to 4" << endl;

  // print all elements
  copy(coll2.cbegin(), coll2.cend(), ostream_iterator<int>(cout, " "));
  cout << endl;

  // remove all elements with value 5
  int num;
  num = coll2.erase(5);
  cout << num << " element with value 5 removed" << endl;

  // print all elements
  copy(coll2.cbegin(), coll2.cend(), ostream_iterator<int>(cout, " "));
  cout << endl;
  // sintersection/union etc.
  // This algorithm works for any ordered renge, not just sets
  set<int> coll3{2, 4, 7, 29, 32, 1, 13, 18};
  // union, we show that detination may be container of other type
  vector<int> allToghether;
  std::set_union(coll2.begin(), coll2.end(), coll3.begin(), coll3.end(),
                 std::back_inserter(allToghether));
  std::cout << "Union:\n";
  std::copy(allToghether.cbegin(), allToghether.cend(),
            ostream_iterator<int>(cout, " "));
  std::cout << std::endl;
  // Intersection. Now I use a set to collect the result
  std::set<int> intersection;
  std::set_intersection(coll2.begin(), coll2.end(), coll3.begin(), coll3.end(),
                        std::inserter(intersection, intersection.begin()));
  std::cout << "Intersection:\n";
  std::copy(intersection.cbegin(), intersection.cend(),
            ostream_iterator<int>(cout, " "));
  std::cout << std::endl;
  // You also have set_difference and merge among algorithms of the same sort
  // The new extract facility
  int  value = 13;
  auto node = intersection.extract(value);
  if(node.empty())
    {
      std::cout << "value " << value << " not present" << std::endl;
    }
  else
    {
      std::cout << "value " << value << " present" << std::endl;
      node.value() = 900; // change value
      auto info = intersection.insert(std::move(node));
      if(info.inserted == false)
        {
          std::cout << " New value  900 not inserted" << std::endl;
        }
      else
        {
          std::cout << " New value 900 inserted" << std::endl;
        }
    }
  std::cout << "Now we have:\n";
  std::copy(intersection.cbegin(), intersection.cend(),
            ostream_iterator<int>(cout, " "));
  std::cout << std::endl;
}
