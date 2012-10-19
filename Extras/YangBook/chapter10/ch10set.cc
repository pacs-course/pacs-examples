#include <algorithm>
#include <set>
//#include <function>
#include <iostream>

using namespace std;

int main() {
  set<int, less<int> > nds;
  nds.insert(5);                   // insert the element 5 into set
  nds.insert(5);                   // insert the element 5. Again? Ignore
  nds.insert(7);                   // insert the element 7 into set
  nds.insert(10);                  // insert the element 10 into set
  nds.erase(7);                    // remove the element 7 from set
  nds.erase(3);                    // 3 is not in set. Ignore

  for (set<int,less<int> >::const_iterator p = nds.begin(); 
                                           p != nds.end(); p++) { 
     cout << *p << "\n";           // print out all elements in set 
  }

  set<int,less<int> >::const_iterator p = nds.find(5);    // is 5 in set?
  if (p != nds.end()) cout << "found = " << *p << "\n";   // output if yes
  set<int,less<int> >::const_iterator q = nds.find(9);    // is 9 in set?
  if (q != nds.end()) cout << "found = " << *q << "\n";   // output if yes

  cout << "number of elements in set = " << nds.size() << "\n";
  cout << "first element in set = " << *(nds.begin()) << "\n";

  set<int,less<int> >::const_iterator t = nds.end();      // last-past-one
  t--;                                                    // last element 
  cout << "last element in set = " << *t << "\n";
} 
