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

int
multiply_with_unrolling(const std::vector<int> &data)
{
  int result = 1;

  int a0, a1, a2, a3, a4;

  for (size_t i = 0; i < data.size() - 4; i += 5)
    {
      a0 = data[i];
      a1 = data[i + 1];
      a2 = data[i + 2];
      a3 = data[i + 3];
      a4 = data[i + 4];

      result *= a0 * a1 * a2 * a3 * a4;
    }

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

  start = timer::now();
  for (size_t i = 0; i < n_iterations; ++i)
    result = multiply_with_unrolling(data);
  end = timer::now();

  std::cout << "Multiply (with unrolling): "
            << std::chrono::duration_cast<std::chrono::milliseconds>(end -
                                                                     start)
                 .count()
            << " ms."
            << ", result: " << result << std::endl;

  return 0;
}
