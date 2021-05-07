#include <iostream>

class Class1
{
private:
  short int s; // 2 bytes.
  int       i; // 4 bytes.
  char      c; // 1 byte.
};

// Layout:
// | s |    s    | padding | padding | // "i" would not fit in this row!
// | i |    i    |    i    |    i    |
// | c | padding | padding | padding |


class Class2
{
private:
  int       i; // 4 bytes.
  short int s; // 2 bytes.
  char      c; // 1 byte.
};

// Layout:
// | i | i | i |    i    |
// | c | s | s | padding |


int
main()
{
  Class1 x1;
  Class2 x2;

  std::cout << "Size of class 1 is " << sizeof(x1) << " bytes." << std::endl;
  std::cout << "Size of class 2 is " << sizeof(x2) << " bytes." << std::endl;

  return 0;
}
