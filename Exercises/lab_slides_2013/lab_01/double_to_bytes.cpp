#include <iostream>
#include <cstdint>

int main (void)
{

  union 
  {
    double d;
    unsigned char b[8];
  }  d;

  d.d = 2.0;
  for (int ii = 7; ii > 0; --ii)
    {      
      std::cout << static_cast<int> (d.b[ii]);
      std::cout << " ";
    }
  std::cout << std::endl;

  return 0;
}




