#include <format>
#include <iostream>
#include <numbers>
int
main()
{
  std::string name = "John";
  int         age = 30;
  double      height = 1.75;

  auto formattedString =
    std::format("Name: {}, Age: {}, Height: {:.2f}", name, age, height);
  std::cout << formattedString << std::endl;
  formattedString =
    std::format("Centered: {:^}, LeftJustified: {:<d}, RightJustified: {:>.2f}",
                name, age, height);
  std::cout << formattedString << std::endl;
  formattedString =
    std::format("With a space of 8 and - as filling\nCentered: {:-^8}, "
                "LeftJustified: {:-<8d}, RightJustified: {:->8.2f}",
                name, age, height);
  std::cout << formattedString << std::endl;
  std::cout << std::format("42 in binary format: {:b}", 42) << std::endl;
  std::cout << std::format("42 in hex format: {:x}", 42) << std::endl;
  std::cout << std::format("Maybe you need to print {{}} before printing {:d}",
                           42)
            << std::endl;
  // different precisiones
  std::cout << std::format("Pi with 2 decimal places: {:.2f}", std::numbers::pi)
            << std::endl;
  std::cout << std::format("Pi with 4 decimal places: {:.4f}", std::numbers::pi)
            << std::endl;
  std::cout << std::format("Pi with 15 decimal places: {:.15f}",
                           std::numbers::pi)
            << std::endl;
  return 0;
}