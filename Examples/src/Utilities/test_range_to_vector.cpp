#include   <iostream>
#include <ranges>
#include <algorithm>
#include "range_to_vector.hpp"
int main()
{
    using namespace Utilities;
    using namespace std::ranges;
    std::vector a{3.,4.,5.,-9.,-3.9};
    // create a view
    auto v = a | views::filter([](auto x){return x>0;});
    // convert the view to a vector
    auto v2 = ranges_to_vector(v);
    // print the vector
    std::ranges::copy(v2,std::ostream_iterator<double>(std::cout, " "));
    std::cout<<std::endl;
}