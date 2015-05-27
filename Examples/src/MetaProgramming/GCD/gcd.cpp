#include "gcd.hpp"
#include <cassert>
namespace Utility
{
  long unsigned int gcd(long unsigned int M, long unsigned int N)
  {
    assert(M != 0ul); // disabled if NDEBUG is acrivated
    if (N==0ul) return M;
    return gcd(N, M%N);
  }
}
