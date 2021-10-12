#include "scientific_precision.hpp"
#include <cmath>
int
main()
{
  using namespace apsc::Utility;
  double      Pid = std::atan2(1., 1.);
  float       Pif = std::atan2(1.f, 1.f);
  long double Pil = std::atan2(1.L, 1.L);
  std::cout << "printed with default precision \n";
  std::cout << "float: " << Pif << std::endl;
  std::cout << "double:" << Pid << std::endl;
  std::cout << "long:  " << Pil << std::endl;
  std::cout << "printed with maximal precision and scientific notation \n";
  std::cout << "float: " << maxPrecFloat << Pif << std::endl;
  std::cout << "double:" << maxPrecDouble << Pid << std::endl;
  std::cout << "long:  " << maxPrecLong << Pil << std::endl;
}
