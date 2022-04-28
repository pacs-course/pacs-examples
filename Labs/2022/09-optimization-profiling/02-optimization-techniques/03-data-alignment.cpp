#include <iostream>

class Class1
{
private:
  short int s; // 2 bytes.
  int       i; // 4 bytes.
  char      c; // 1 byte.
};

int
main()
{
  Class1 x1;

  std::cout << "Size of class 1 is " << sizeof(x1) << " bytes." << std::endl;

  return 0;
}
