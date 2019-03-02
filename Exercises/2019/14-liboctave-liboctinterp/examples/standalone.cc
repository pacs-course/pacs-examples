#include <iostream>
#include <octave/oct.h>

int main (void)
{

  Matrix A (4, 4);
  for (octave_idx_type i = 0; i < 4; i++)
    for (octave_idx_type j = 0; j < 4; j++)
      A(i,j) = 1.0 / (static_cast<double> (i) +
                      static_cast<double> (j) + 1.0);

  ColumnVector b (4, 1.0);
  ColumnVector x = A.solve (b);

  std::cout << "A = " << std::endl << A << std::endl
            << "b = " << std::endl << b << std::endl
            << "x = " << std::endl << x << std::endl;

  return 0;
}
