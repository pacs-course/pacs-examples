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
#include <deque>
#include <iostream>
#include <iterator>
#include <string>
using namespace std;

int
main()
{
  // create empty deque of strings
  deque<string> coll;

  // insert several elements
  coll.assign(3, string("string"));
  coll.push_back("last string");
  coll.push_front("first string");

  // print elements separated by newlines
  copy(coll.cbegin(), coll.cend(), ostream_iterator<string>(cout, "\n"));
  cout << endl;

  // remove first and last element
  coll.pop_front();
  coll.pop_back();

  // insert "another" into every element but the first
  for(unsigned i = 1; i < coll.size(); ++i)
    {
      coll[i] = "another " + coll[i];
    }

  // change size to four elements
  coll.resize(4, "resized string");

  // print elements separated by newlines
  copy(coll.cbegin(), coll.cend(), ostream_iterator<string>(cout, "\n"));
}
