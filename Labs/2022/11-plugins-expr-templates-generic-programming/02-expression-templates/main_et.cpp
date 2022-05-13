#include <cassert>
#include <iostream>
#include <vector>

template <class T, class Container = std::vector<T>>
class MyVector
{
public:
  // MyVector with initial size.
  MyVector(const size_t &n)
    : cont(n)
  {}

  // MyVector with initial size and value.
  MyVector(const size_t &n, const double &initial_value)
    : cont(n, initial_value)
  {}

  // Constructor for underlying container.
  MyVector(const Container &other)
    : cont(other)
  {}

  // Assignment operator for MyVector of different type.
  template <class T2, class R2>
  MyVector &
  operator=(const MyVector<T2, R2> &other)
  {
    assert(size() == other.size());

    for (size_t i = 0; i < cont.size(); ++i)
      cont[i] = other[i];

    return (*this);
  }

  size_t
  size() const
  {
    return cont.size();
  }

  T
  operator[](const size_t &i) const
  {
    return cont[i];
  }

  T &
  operator[](const size_t &i)
  {
    return cont[i];
  }

  const Container &
  data() const
  {
    return cont;
  }

  Container &
  data()
  {
    return cont;
  }

private:
  Container cont;
};

// A program that evaluates (x + x + y * y).
int
main(int argc, char **argv)
{
  constexpr size_t N = 1e8;

  MyVector<double> x(N, 5.4);
  MyVector<double> y(N, 10.3);

  // Compute x + x + y * y.

  return 0;
}
