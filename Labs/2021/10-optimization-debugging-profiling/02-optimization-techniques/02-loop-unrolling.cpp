#include <chrono>
#include <iostream>
#include <vector>

const size_t vec_size = 1000;

int
multiply(const std::vector<int> &data)
{
  int result = 1;

  for (size_t i = 0; i < data.size(); ++i)
    result *= data[i];

  return result;
}

using timer = std::chrono::high_resolution_clock;

int
main(int argc, char **argv)
{
  size_t n_iterations = 1e7;

  // Generate an arbitrary dataset.
  auto data = std::vector<int>(vec_size, 1);

  auto   start = timer::now();
  double result;
  for (size_t i = 0; i < n_iterations; ++i)
    result = multiply(data);
  auto end = timer::now();

  std::cout << "Multiply: "
            << std::chrono::duration_cast<std::chrono::milliseconds>(end -
                                                                     start)
                 .count()
            << " ms."
            << ", result: " << result << std::endl;

  return 0;
}
