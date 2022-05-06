#include <iostream>

class A
{
public:
  A(const int &i)
    : num(i)
  {
    // Here num is equal to 2.
    std::cout << num << std::endl;
  }

  void
  print()
  {
    // Here accessing num results in undefined behaviour!
    std::cout << num << std::endl;
  }

private:
  const int &num;
};

int
main()
{
  // We are binding a const reference to a temporary object.
  A a(2);

  a.print();

  return 0;
}
