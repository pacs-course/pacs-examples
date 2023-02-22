#include <array>
#include <iostream>
/*!
 * This is an aggregate
 */
struct aggr
{
  double a;
  int    b;
};

std::ostream &
operator<<(std::ostream &out, aggr const &a)
{
  out << "Object ot type aggr with a=" << a.a << " and b=" << a.b << std::endl;
  return out;
}
/*!
 * an array is an aggregate!
 * @return an array of 2 ints
 */
std::array<int, 2>
anArray()
{
  return {1, 2};
}

/*!
 * Overloading streaming operator to print arrays (it can be made better)
 * @tparam T The type contained in the array
 * @tparam N The size of the array
 * @param out the output stream
 * @param myarray The array
 * @return the output stream
 */
template <class T, std::size_t N>
std::ostream &
operator<<(std::ostream &out, std::array<T, N> const & myarray)
{
  out << "Array dimension:" << N << "\n";
  out << "Array content" << std::endl;
  for(auto const & i : myarray)
    {
      out << i << " ";
    }
  out << std::endl;
  return out;
}

/*!
 * A function that takes a pointer to an array of double, the old C-style for
 * arrays (and C++98 style). To show how you can use C++ standard arrays and vectors
 * with legacy code
 * @param a a pointer to a C array
 * @param n The size of the array
 */
void
fun(double const *a, int n)
{
  std::cout << "C-Array dimension:" << n << "\n";
  std::cout << "C-Array content:\n";
  for(int i = 0; i < n; ++i)
    {
      std::cout << "a[" << i << "]=" << a[i] << " ";
    }
  std::cout << std::endl;
}

int
main()
{
  using namespace std;
  array<int, 3> b{1, 2, 3}; // brace initialization (array is an aggregate!!)
  std::cout << b;
  array<double, 3> c = {1., 2., 3.}; // brace-equal initialization (internal braces may be omitted)
  // An array of aggregates is an aggregate! So I can use list initialization.
  // If I use this version I need to start with double brackets... I will not
  // explain why here, just believe me.
  array<aggr, 3> bb{{{5.0, 3}, {6.3, 4}, {-1.0, 7}}};
  std::cout << bb;
  // But you can also avoid the double and the internal brackets
  array<aggr, 3> bb2{5.0, 3, 6.3, 4, -1.0, 7};
  std::cout << bb2;
  // you can retreive values with structured bindings
  auto [x, y] = anArray();
  std::cout << "x= " << x << " y=" << y << std::endl;
  // You can traverse an array like any other standard containes using iterators
  // or range based for loops
  for(auto i : bb)
    {
      std::cout << i << std::endl;
    }
  // You can pass the array as a C-array using the method data()
  fun(c.data(), 3);
}
