#include <complex-step.h>
#include <iomanip>



int
main ()
{

  auto SinCos = [] (auto x) { return std::sin (std::cos (x)); };
  auto d_SinCos_d_x = diff (SinCos);
  
  for (double x = 0.0; x < 2.0 * M_PI; x += 0.001)
    std::cout << std::setprecision (15) << std::scientific << x << " "
              << - std::cos (std::cos (x)) * std::sin (x) << " " << diff (SinCos, x)  << " " << d_SinCos_d_x (x) << " "
              << - std::cos (std::cos (x)) * std::sin (x) - diff (SinCos, x)
              << std::endl;
  
  return 0;
}
