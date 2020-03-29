// [form and print a list of integers]
#include <iostream>
#include "integer-list.hpp"

int main (int argc, char** argv)
{
  using namespace std;
  int c = 1;

  // create a new list
  Node start (c);

  if (start.isLast ())
    std::cout << "this is the last node" << std::endl;

  if (start.isFirst ())
    std::cout << "this is the first node" << std::endl;
  
  // add 10 nodes to the list
  while (c < 10)
    {
      c++;
      start.appendNew (c);
    }

  Node* newnode = new Node ();
  newnode->setData (11);
  
  start.append (newnode);

  std::cout << "the new node is attached to a node with data = "
            << newnode->getPrevious ()->getData () << std::endl;
    
  std::cout << "the list has " << start.size ()
            << " nodes" << std::endl;
  
  // print the list
  start.print ();

  // find the node with value 5
  Node* t = start.find (5);

  // erase this node
  t->erase ();

  // find the node with value 8
  Node* tt = start.find (8);
  
  // erase this node
  tt->replaceWith (new Node(12));
  
  // print the list again
  start.print ();

  return 0;
}
