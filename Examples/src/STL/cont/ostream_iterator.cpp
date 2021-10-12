#include <algorithm> // for copy
#include <iostream>
#include <iterator> // for stream iterator
#include <vector>
//! A simple example of use of stream iterators
int
main()
{
  std::ostream_iterator<int> out_it(std::cout, ", ");
  std::vector<int>           vals{1, 2, 3, 4, 5, 6};
  std::copy(begin(vals), end(vals), out_it);
  std::cout << std::endl;
}
