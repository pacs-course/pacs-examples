#ifndef HH__ABSDIFF__HH__
#define HH__ABSDIFF__HH__
#include <type_traits>
namespace apsc
{
  template<class T1, class T2>
  constexpr auto absdiff(T1 const & a, T2 const & b)
  {
    using namespace std;
    static_assert(
      (is_signed_v<T1>   and is_signed_v<T2>) or
      (is_unsigned_v<T1> and is_unsigned_v<T2>),
      "absdiff error: arguments must be both of unsigned or signed type");
    return a>b?a-b:b-a;
  }
 }
#endif
