// Build and print a list of integers.
#include "integer-list.hpp"

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

  // print the list
  start.print();

  // find the node with value 5
  Node *t = start.find(5);

  // erase this node
  t->erase();

  // print the list again
  start.print();

  return 0;
}
