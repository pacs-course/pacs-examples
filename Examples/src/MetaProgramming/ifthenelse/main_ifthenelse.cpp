#include "ifthenelse.hpp"
#include <iostream>
#include <limits>
#include <type_traits>
#include <vector>
// Returns a long double if T is floating type, and a long int if T is an
// integral type
template <typename T>
apsc::TypeTraits::IfThenElse_t<std::is_floating_point<T>::value, long double,
                               long int>
bigsum(std::vector<T> x)
{
  using namespace apsc::TypeTraits;
  //! fails if it is not a float or an integer type
  static_assert(std::is_floating_point<T>::value || std::is_integral<T>::value,
                "Error");
  using value_type =
    IfThenElse_t<std::is_floating_point<T>::value, long double, long int>;
  value_type init{0};
  for(auto i : x)
    init += static_cast<value_type>(i);
  return init;
}

int
main()
{
  int                 bigint{std::numeric_limits<int>::max() - 1};
  double              bigdouble{std::numeric_limits<double>::max() - 1.0};
  std::vector<int>    a{bigint, bigint, bigint};
  std::vector<double> b{bigdouble, bigdouble, bigdouble};

  std::cout << " Results with bigsum" << std::endl;
  std::cout << bigsum(a) << std::endl;
  std::cout << bigsum(b) << std::endl;
}
