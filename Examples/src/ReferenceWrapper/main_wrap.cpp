#include "refwrap.hpp"
#include <algorithm>
#include <iostream>
#include <list>
#include <memory>
#include <utility>
#include <vector>
int
main()
{
  // This example is pratically useless. It is here only to show ref()
  // In more practical situations, if you have a function whose
  // arguments are passed by value, you may want to use ref() of cref()
  // to save a copy. But beware you may change the value of the
  // argument unless you use cref(), then if the function changes
  // the value of the argument you have a compiler error.

  double a = 10;
  apsc::apply(apsc::timesThree, a);
  // a has not changed since it is passed by value!
  std::cout << "a=" << a << std::endl;
  apsc::apply(apsc::timesThree, std::ref(a));
  // Now a has changed
  std::cout << "a=" << a << std::endl;
  std::list<double> l{1., 2., 3.};
  // A vector of references to elements of l
  std::vector<std::reference_wrapper<double> > v(l.begin(), l.end());
  std::cout << "Values of v\n";
  for(auto i : v)
    std::cout << i << " ";
  std::cout << std::endl;
  // Change the elements of l (again a use of std::ref!! since
  // timesThree takes a value not a reference)
  // Note: you need auto& since you want to change the element of l!
  for(auto &i : l)
    apsc::timesThree(std::ref(i));
  // also the elements of v have changed, being reference
  // wrappers!
  std::cout << "Values of v\n";
  for(auto i : v)
    std::cout << i << " ";
  std::cout << std::endl;
}
