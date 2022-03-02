// A linked-list of integers
#include <iostream>

class Node
{
public:
  // constructors / destructor
  Node()
    : next(nullptr)
    , previous(nullptr)
  {}

  Node(int a)
    : next(nullptr)
    , previous(nullptr)
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
  getData()
  {
    return data;
  }

  void
  setNext(Node *theNext)
  {
    next = theNext;
  }

  Node *
  getNext()
  {
    return next;
  }

  void
  setPrevious(Node *thePrevious)
  {
    previous = thePrevious;
  }

  Node *
  getPrevious()
  {
    return previous;
  }

  // list capabilities
  // return true if node is the first of the list, false otherwise
  bool
  isFirst()
  {
    return !previous;
  }

  // return true if node is the last of the list, false otherwise
  bool
  isLast()
  {
    return !next;
  }

  // return the size of the sublist starting from this node
  int
  size()
  {
    Node *t   = this;
    int   ret = 1;
    while (!t->isLast())
      {
        t = t->getNext();
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
      t = t->getNext();

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
      t = t->getNext();

    Node *theNewNode = new Node(a);
    t->setNext(theNewNode);
    theNewNode->setPrevious(t);
  }

  // remove this node from the list
  void
  erase()
  {
    previous->setNext(next);
    next->setPrevious(previous);
    next = nullptr;
    delete this;
  }

  // replace this node with a given node
  void
  replaceWith(Node *replacement)
  {
    previous->setNext(replacement);
    next->setPrevious(replacement);
    replacement->setNext(next);
    replacement->setPrevious(previous);
    next = nullptr;
    delete this;
  }

  // find first node with a specified value in sublist starting from
  // this node return nullptr if not found
  Node *
  find(int value)
  {
    if (data == value)
      return this;

    Node *t = this;
    while (!t->isLast())
      {
        t = t->getNext();
        if (t->getData() == value)
          return t;
      }

    return nullptr;
  }

  // print the data in the sublist starting from this node
  void
  print()
  {
    Node *t = this;
    while (!t->isLast())
      {
        std::cout << t->getData() << ", ";
        t = t->getNext();
      }
    std::cout << t->getData() << std::endl;
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
