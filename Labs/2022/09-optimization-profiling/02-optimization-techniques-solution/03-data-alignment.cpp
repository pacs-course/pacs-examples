#include <iostream>
#include <vector>

/// Layout:
/// | s |    s    | padding | padding | // "i" would not fit in this row!
/// | i |    i    |    i    |    i    |
/// | c | padding | padding | padding |
class Class1
{
private:
  short int s; // 2 bytes.
  int       i; // 4 bytes.
  char      c; // 1 byte.
};


/// Layout:
/// | i | i | i |    i    |
/// | c | s | s | padding |
class Class2
{
private:
  int       i; // 4 bytes.
  short int s; // 2 bytes.
  char      c; // 1 byte.
};


int
main()
{
  const size_t length = 1e8;

  const std::vector<Class1> x1(length);
  const std::vector<Class2> x2(length);

  // Conversion factor from kB to GB.
  const double conv = 1024 * 1024 * 1024;

  std::cout << "Size of x1 is " << x1.size() * sizeof(Class1) / conv << " GB."
            << std::endl;
  std::cout << "Size of x2 is " << x2.size() * sizeof(Class2) / conv << " GB."
            << std::endl;

  return 0;
}
