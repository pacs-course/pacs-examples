#include <iostream>
#include <list>
#include<algorithm>

using namespace std;

int main() {

  list<int> nodes;                   // a list of integers for nodes

  nodes.push_front(10);              // add 10 at the beginning
  nodes.push_back(5);                // add 5 at the end
  nodes.push_back(6);                // add 6 at the end
  nodes.push_back(4);                // add 4 at the end
  nodes.push_back(4);                // add 4 at the end
  nodes.push_back(4);                // add 4 at the end
  nodes.pop_back();                  // remove last element
  nodes.remove(10);                  // remove element 10

  for (list<int>::iterator j = nodes.begin(); j != nodes.end(); j++) 
    cout << *j << "  ";              // *j is the element at position j
  cout << endl;  

  nodes.sort();                      // sort nodes in increasing order
  for (list<int>::iterator j = nodes.begin(); j != nodes.end(); j++) 
    cout << *j << "  ";              // *j is the element at position j
  cout << endl;  

  nodes.unique();                    // remove duplicate elements in list
  for (list<int>::iterator j = nodes.begin(); j != nodes.end(); j++) 
    cout << *j << "  ";              // *j is the element at position j
  cout << endl;  

  nodes.reverse();                   // reverse the order of elements
  for (list<int>::iterator j = nodes.begin(); j != nodes.end(); j++) 
    cout << *j << "  ";              // *j is the element at position j
  cout << endl;  

  list<int>::iterator i = find(nodes.begin(), nodes.end(), 6);    
      // find position of element 6 
  nodes.insert(i, 2);                // insert before element that i refers to
  nodes.insert(i, 5, 7);             // insert 5 copies of 7
  nodes.erase(i);

  for (list<int>::iterator j = nodes.begin(); j != nodes.end(); j++) 
    cout << *j << "  ";              // *j is the element at position j
  cout << endl;  

  nodes.sort();
  nodes.unique();
  for (list<int>::const_iterator j = nodes.begin(); j != nodes.end(); j++) 
    cout << *j << "\n"; 

  // merge() and splice()
  list<double> ft;                   // a list of integers for nodes
  list<double> sd;                   // a list of integers for nodes
  ft.push_front(5.5);
  ft.push_front(5.9);
  ft.push_front(5.6);

  sd.push_front(3.5);
  sd.push_front(3.9);
  sd.push_front(.6);
  ft.sort();
  sd.sort();
  ft.merge(sd);               // ft has the merged list, sd will be empty

  for (list<double>::iterator j = ft.begin(); j != ft.end(); j++) 
    cout << *j << "\n";            // *j is the element at position j
  cout << endl;  

  list<double>::iterator ii = find(ft.begin(), ft.end(), 5.5); 
  sd.splice(sd.begin(), ft, ii);     
          // take element at position ii of ft and insert it to sd at front 

  for (list<double>::iterator j = sd.begin(); j != sd.end(); j++) 
    cout << *j << "\n";            // *j is the element at position j
  cout << endl;  

  sd.splice(sd.end(), ft);     
          // take all elements of ft and insert them in sd at back

  for (list<double>::iterator j = sd.begin(); j != sd.end(); j++) 
    cout << *j << "\n";            // *j is the element at position j
}
