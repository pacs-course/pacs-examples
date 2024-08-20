#include "parallel_for.hpp"
#include <iostream>

// Test case 1: Parallel loop with lambda function
void
test_case_1()
{
  std::cout << "Test case 1:\n";
  std::vector sum = {1, -1, 3, 4, 7, 9, 11};
  auto        add_to_sum = [&sum](int i) { sum[i] += 2; };
  apsc::parallel_for(std::execution::par, std::size_t{0}, sum.size(),
                     add_to_sum);
  std::cout << "computed values " << std::endl;
  for(int i : sum)
    {
      std::cout << i << " ";
    }
  std::cout << std::endl;
}

// Test case 2: Parallel loop with function object
struct MultiplyByTwo
{
  std::vector<int> v;
  void
  operator()(int i)
  {
    v.push_back(i * 2);
  }
};

void
test_case_2()
{
  std::cout << "Test case 2:\n";
  MultiplyByTwo m;
  apsc::parallel_for(std::execution::par, 0, 5, std::ref(m));
  for(int i : m.v)
    {
      std::cout << i << " ";
    }
  std::cout << std::endl;
  // Expected output: 0 2 4 6 8
}

int
main()
{
  test_case_1();
  test_case_2();
  return 0;
}