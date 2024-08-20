#include <iostream>
#include <span>
#include <vector>

void
printSpan(std::span<int> span)
{
  for(int element : span)
    {
      std::cout << element << " ";
    }
  std::cout << std::endl;
}

int
main()
{
  // a C=style array
  int numbers[] = {1, 2, 3, 4, 5};

  // Create a span from the array
  std::span<int> span(numbers);

  // Print the elements of the span
  printSpan(span);

  // Modify the elements of the span
  for(int &element : span)
    {
      element *= 2;
    }

  // Print the modified elements of the span
  printSpan(span);

  std::cout << "The original array is also modified\n";
  // and I can print it using printSpan!
  printSpan(numbers);
  return 0;
}