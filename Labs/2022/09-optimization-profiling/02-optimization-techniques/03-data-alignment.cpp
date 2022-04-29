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
  std::cout << "Size of Class1 is " << sizeof(Class1) << " bytes."
            << std::endl;

  return 0;
}
