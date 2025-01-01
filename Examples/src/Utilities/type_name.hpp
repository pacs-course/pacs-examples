#pragma once
// code based on
// https://stackoverflow.com/questions/81870/is-it-possible-to-print-a-variables-type-in-standard-c
#include <string_view>
/*!
@brief Get the type name of a variable
@tparam T Type of the variable
@return The type name of the variable
@note possible usage:
@code
template <class T>
void foo(const T& variable)
{
    std::cout << type_name<T>() << std::endl;
}
@endcode
*/
namespace apsc
{
template <class T>
constexpr std::string_view
type_name()
{
#ifdef __clang__
  std::string_view p = __PRETTY_FUNCTION__;
  return std::string_view(p.data() + 34, p.size() - 34 - 1);
#elif defined(__GNUC__)
  std::string_view p = __PRETTY_FUNCTION__;
#if __cplusplus < 201402
  return std::string_view(p.data() + 36, p.size() - 36 - 1);
#else
  return std::string_view(p.data() + 49, p.find(';', 49) - 49);
#endif
#endif
}
} // namespace apsc