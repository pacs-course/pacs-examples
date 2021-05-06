// A linked-list of integers
#include <iostream>

class Node
{
public:
  // constructors / destructor
  Node()
    : next(NULL)
    , previous(NULL)
  {}

  Node(int a)
    : next(NULL)
    , previous(NULL)
    , data(a)
  {}

  ~Node()
  {
    if (next)
      delete next;
  }

  // set/get interface
  void
  setData(int a)
  {
    data = a;
  }

  int
  getData(void)
  {
    return data;
  }

  void
  setNext(Node *theNext)
  {
    next = theNext;
  }

  Node *
  getNext(void)
  {
    return next;
  }

  void
  setPrevious(Node *thePrevious)
  {
    previous = thePrevious;
  }

  Node *
  getPrevious(void)
  {
    return previous;
  }

  // list capabilities
  // return true if node is the first of the list, false otherwise
  bool
  isFirst(void)
  {
    return !previous;
  }

  // return true if node is the last of the list, false otherwise
  bool
  isLast(void)
  {
    return !next;
  }

  // return the size of the sublist starting from this node
  int
  size(void)
  {
    Node *t   = this;
    int   ret = 1;
    while (!t->isLast())
      {
        t = next;
        ret++;
      }
    return ret;
  }

  // append a new given node at the end of the list
  void
  append(Node *theNext)
  {
    Node *t = this;
    while (!t->isLast())
      t = next;

    t->setNext(theNext);
    theNext->setPrevious(t);
  }

  // create a new node with value 'a' and append it at the end of the
  // list
  void
  appendNew(int a)
  {
    Node *t = this;
    while (!t->isLast())
      t = next;

    Node *theNewNode = new Node(a);
    t->setNext(theNewNode);
    theNewNode->setPrevious(t);
  }

  // remove this node from the list
  void
  erase(void)
  {
    previous->setNext(next);
    next->setPrevious(previous);
  }

  // replace this node with a given node
  void
  replaceWith(Node *replacement)
  {
    previous->setNext(replacement);
    next->setPrevious(replacement);
  }

  // find first node with a specified value in sublist starting from
  // this node return NULL if not found
  Node *
  find(int value)
  {
    if (data == value)
      return this;

    Node *t = this;
    while (!t->isLast())
      {
        t = next;
        if (t->getData() == value)
          return t;
      }

    return NULL;
  }

  // print the data in the sublist starting from this node
  void
  print(void)
  {
    Node *t = this;
    while (!t->isLast())
      {
        std::cout << t.getData() << ", ";
        t = next;
      }
    std::cout << t.getData() << std::endl;
  }

protected:
  // pointer to next node in list
  Node *next;

  // pointer to previous node in list
  Node *previous;

private:
  // the integer data stored in the node
  int data;
};
