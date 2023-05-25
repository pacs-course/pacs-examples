//
// Created by forma on 22/05/23.
//

#ifndef EXAMPLES_STONUM_HPP
#define EXAMPLES_STONUM_HPP

#include <optional>
#include <string>
#include <sstream>
#include <algorithm>
#include <type_traits>
#include <cctype>
#include "string_utility.hpp"
// Taken from https://www.cppstories.com/2018/06/optional-examples-wall/
  // Converts a text number to specified type.
  // All of the text must be a valid number of the specified type.
  // eg 63q is invalid
  // Defaults to type int
  // st - string to convert
  // returns either value of converted number or
  // no value if text number cannot be converted
namespace Utility
{
/*!
 * @brief Checks if a character is a digit.
 * @details
 * Needed to avoid some shortcomings of std::isdigit, see
 * https://en.cppreference.com/w/cpp/string/byte/isdigit
 * @param ch The character to check
 * @return true if the character is a digit, false otherwise
 */
bool myIsdigit(unsigned char ch)
{
    return std::isdigit(ch);
}
/*!
 * @brief Converts a text number to specified type.
 * @details
 * It is an example of use of std::optional taken from
 * https://www.cppstories.com/2018/06/optional-examples-wall/
 *
 * Example of use:
 *
 * @code
 * #include <iostream>
 * #include <string>
 * #include "stnum.hpp"
 * ...
 * // reading a line with the number
 * getline(file,s)
 * // extracting the number
 * auto n = Utility::stonum<int>(s);
 * if(n) {
 *  // we have a number
 *  std::cout << "The number is " << *n << std::endl;
 *  }
 *  @endcode
 * *
 * @tparam T The type of number we want to extract
 * @param st The string with the number values
 * @return If the string contains a number of type T, it returns the number
 *        otherwise it returns an empty optional
 */
template <typename T = int>
std::optional<T>
stonum(const std::string &st)
{
  const auto s = Utility::trim(st);
  // Check the format of the string
  bool       ok = !s.empty() && (myIsdigit(s.front()) ||
                           (((std::is_signed_v<T> && (s.front() == '-')) ||
                             (s.front() == '+')) &&
                            ((s.size() > 1u) && myIsdigit(s[1]))));

  auto v = T{};

  if(ok)
    {
      std::istringstream ss(s);

      ss >> v;
      // check if we have read the whole string
      ok = !ss.fail();
    }
  return ok ? v : std::optional<T>{};
}
} // namespace Utility
#endif // EXAMPLES_STONUM_HPP
