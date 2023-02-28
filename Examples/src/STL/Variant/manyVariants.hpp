//
// Created by forma on 28/02/23.
//

#ifndef EXAMPLES_MANYVARIANTS_HPP
#define EXAMPLES_MANYVARIANTS_HPP
#include <variant>
#include <string>
#include <iostream>
namespace apsc
{
// Some classes, just to define types
struct Fluid { };
struct LightItem { };
struct HeavyItem { };
struct FragileItem { };
using ItemsType=std::variant<Fluid,LightItem,HeavyItem,FragileItem>;

// Some classes, just to define types
struct CartonBox{ };
struct PlasticBox{ };
struct MetalBox{ };
using BoxType=std::variant<CartonBox,PlasticBox,MetalBox>;
/*!
 * A funny visitor. Itprints a message depending on the match of
 * Box and Item types.
 *
 * An overloaded set of operators
 *
 * @note The use of automatic functions to define a "default action:.
 */
struct checkParcel
{
  void operator()(CartonBox const &, Fluid const &)
  {
  std::cout<<"You cannot put fluids in a carton box!\n";
  }
  void operator()(CartonBox const &, HeavyItem const &)
  {
  std::cout<<"Item too heavy, carton box breaks into pieces!\n";
  }
  // The example of a default
  void operator()(auto const &, FragileItem const &)
  {
  std::cout<<"Handle with care!!\n";
  }
  void operator()(MetalBox const &, Fluid const &)
  {
  std::cout<<"Box gets rusty!!\n";
  }
  // the final default
   void operator()(auto const &, auto const &)
  {
  std::cout<<"Transport is safe!!\n";
  }
};

/*!
 * Here an example on how you can pass an additional argument.
 * Other techniques are possible, see
 * <a href="https://www.cppstories.com/2018/09/visit-variants/">here</a>
 */
struct ItemsAndQuantities {
    //ItemsAndQuantities(int p) : val_(p) { }// infact not needed. This way it's an aggregate!

    void operator()(CartonBox const &) { std::cout<<" "<<val_<<" Carton Boxes\n"; }
    void operator()(MetalBox const &) {std::cout<<" "<<val_<<" Metal Boxes\n"; }
    void operator()(PlasticBox const & ) { std::cout<<" "<<val_<<" Plastic Boxes\n"; }

    int val_ { 0 };
};

}

#endif // EXAMPLES_MANYVARIANTS_HPP
