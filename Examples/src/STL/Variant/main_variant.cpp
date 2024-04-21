#include "SimpleVisitor.hpp"
#include "manyVariants.hpp"
#include <algorithm>
#include <iostream>
#include <string>
#include <tuple>
#include <type_traits>
#include <variant>
#include <vector>

/*! This part is only for the nerds of you (skip if you are not interested)
 * This is a class with which you can overload an arbitrary number of functors
 *  It is contained in the file overloaded.hpp in Utilities.
 *  It is repeated here only for convenience
 */
template <class... Ts> struct overloaded : Ts...
{
  using Ts::operator()...;
};
// explicit deduction: it allows to create an
// overloaded object just using the constructor.
// It is not necessary since C+=20.
template <class... Ts> overloaded(Ts...) -> overloaded<Ts...>;
// End nerdish part

int
main()
{
  using namespace apsc;
  // A vector of Variant1
  std::vector<Variant1> v;
  v.emplace_back(3);
  v.emplace_back(7.0);
  v.emplace_back("Hello World");
  using namespace std::complex_literals;
  v.emplace_back(1.0 + 3.5i);
  for(auto k : v)
    std::cout << k;
  // now visit each variant
  // The correct overloaded call operator is called on the variant
  std::cout << "\n\n";
  std::cout << "\n\n";
  for(auto i : v)
    std::visit(SimpleVisitor{}, i);

  // nerdish part: I construct the visitor using overloaded
  // I show also how to make a default!
  overloaded visitor{
    [](int i) { std::cout << "it's an integer equal to " << i << std::endl; },
    [](const std::string &i) {
      std::cout << "it's a string equal to " << i << std::endl;
    },
    [](auto const &) { std::cout << "it's neither an int nor a string\n"; }};

  std::cout << "\n\n";
  int count = 0;
  for(auto k : v)
    {
      std::cout << "Element n." << count++ << " of v ";
      std::visit(visitor, k);
    }
  // end nerdish section

  // Now a visitor that takes two variants!

  std::vector<std::pair<BoxType, ItemsType>> parcels;
  parcels.emplace_back(MetalBox{}, Fluid{});
  parcels.emplace_back(PlasticBox{}, Fluid{});
  parcels.emplace_back(MetalBox{}, HeavyItem{});
  parcels.emplace_back(CartonBox{}, Fluid{});
  parcels.emplace_back(CartonBox{}, LightItem{});

  std::cout << std::endl << std::endl;

  for(auto const &k : parcels)
    std::visit(checkParcel{}, k.first, k.second);

  // Just to show tuples
  std::vector<std::tuple<BoxType, int>> numberOfBoxes;
  numberOfBoxes.emplace_back(CartonBox{}, 10);
  numberOfBoxes.emplace_back(MetalBox{}, 3);
  numberOfBoxes.emplace_back(PlasticBox{}, 1);
  std::cout << "\n\n I have:\n";
  for(auto const &[boxtype, number] : numberOfBoxes)
    {
      std::visit(ItemsAndQuantities{number}, boxtype);
    }
}
