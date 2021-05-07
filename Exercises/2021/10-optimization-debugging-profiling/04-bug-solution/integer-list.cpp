// Build and print a list of integers.
#include "integer-list.hpp"

#include <iostream>

int
main(int argc, char **argv)
{
  int c = 1;

  // create a new list
  Node start(c);

  // add 10 nodes to the list
  while (c < 10)
    {
      c++;
      start.appendNew(c);
    }

  // append node with value 11
  Node *newnode = new Node();
  newnode->setData(11);

  start.append(newnode);

  std::cout << "the new node " << newnode->getData()
            << " is attached to a node with data = "
            << newnode->getPrevious()->getData() << std::endl;

  std::cout << "the list has " << start.size() << " nodes"
            << std::endl;

  // print the list
  start.print();

  // find the node with value 5 and erase it
  Node *t = start.find(5);
  t->erase();

  // find the node with value 8 and replace it
  // with a new node with value 12
  Node *tt = start.find(8);
  tt->replaceWith(new Node(12));

  // print the list again
  start.print();

  return 0;
}
