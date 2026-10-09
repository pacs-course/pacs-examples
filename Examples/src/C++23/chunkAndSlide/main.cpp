#include <algorithm>
#include <iostream>
#include <numeric> // for accumulate
#include <ranges>
#include <vector>
int
main()
{
  std::vector<int> v{1, 2, 3, 4, 5, 6, 7, 8, 9};
  // print the original vector
  std::cout << "Original vector: ";
  for(auto elem : v)
    std::cout << elem << " ";
  std::cout << "\n";
  // compute the sum of each chunk of 3 elements
  std::cout << "Sum of each chunk of 3 elements: ";
  auto rng = v | std::views::chunk(3) | std::views::transform([](auto &&chunk) {
               return std::accumulate(chunk.begin(), chunk.end(), 0);
             });
  for(auto sum : rng)
    std::cout << sum << " ";
  std::cout << "\n";

  // compute the sum of each sliding window of 3 elements
  std::cout << "Sum of each sliding window of 3 elements: ";
  auto slides = v | std::views::slide(3);
  auto windows = slides | std::views::transform([](auto &&window) {
                   return std::accumulate(window.begin(), window.end(), 0);
                 });
  for(auto sum : windows)
    std::cout << sum << " ";
  std::cout << "\n";
  // print the vector with stride 2
  std::cout << "Vector with stride 2: ";
  auto stride = v | std::views::stride(2);
  for(auto elem : stride)
    std::cout << elem << " ";
  std::cout << "\n";
}