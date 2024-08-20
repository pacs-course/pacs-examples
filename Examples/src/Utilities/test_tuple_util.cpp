#include "tuple_utilities.hpp"
#include <complex>
#include <iostream>
#include <string>
#include <tuple>

struct functor
{
  double
  operator()(double x) const
  {
    return x + 1;
  }
  std::complex<double>
  operator()(std::complex<double> x) const
  {
    return x + std::complex<double>(1, 1);
  }
};

int
main()
{
  // Test case 1: Applying a function to each element of a tuple
  std::tuple<int, double, std::string> myTuple(1, 2.5, "Hello");
  auto                                 printElement = [](const auto &element) {
    std::cout << element << std::endl;
  };
  apsc::for_each2(myTuple, printElement);
  // Output: 1, 2.5, Hello

  // Test case 2: Applying a function to each element of an empty tuple
  std::tuple<> emptyTuple;
  auto incrementElement = [](const auto &element) { return element + 1; };
  auto emptyResult = apsc::for_each(emptyTuple, incrementElement);
  // Output: (empty tuple
  // print the size of the resulting tuple
  std::cout << "The resulting tuple has "
            << std::tuple_size_v<decltype(emptyResult)> << " elements\n";
  // now with a function object
  std::tuple<double, std::complex<double>> myTuple2(1.0,
                                                    std::complex<double>(1, 0));
  functor                                  f;
  auto result2 = apsc::for_each(myTuple2, f);
  // printour results2
  std::cout << "The results of the function object are\n";
  apsc::for_each2(result2, printElement);

  auto mytuple = std::make_tuple(1, 2.5, "Hello");
  auto result1 = apsc::tuple_all_of(mytuple, [](const auto &element) {
    return std::is_arithmetic_v<std::remove_cvref_t<decltype(element)>>;
  });
  std::cout << "All elements of myTuple are arithmetic types: "
            << std::boolalpha << result1 << std::endl;

  auto mytuple2 = std::make_tuple(1, 2.5, 4.0f);
  // auto mytuple2 = std::make_tuple(1.0);

  result1 = apsc::tuple_all_of(mytuple2, [](const auto &element) {
    auto res = std::is_arithmetic_v<std::remove_cvref_t<decltype(element)>>;
    return res;
  });
  std::cout << "All elements of myTuple are arithmetic types: "
            << std::boolalpha << result1 << std::endl;

  return 0;
}