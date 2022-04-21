/*
 * overloaded.hpp
 *
 *  Created on: Apr 21, 2022
 *      Author: forma
 */

#ifndef EXAMPLES_SRC_UTILITIES_OVERLOADED_HPP_
#define EXAMPLES_SRC_UTILITIES_OVERLOADED_HPP_
namespace apsc
{
  // helper type for the visitor
  /*!
   * @struct overloaded
   * @brief helper type for a visitor. It implements the overloaded pattern
   *
   * Usage:
   * @code
    using var_t = std::variant<int, long, double, std::string>;
    std::vector<var_t> vec = {10, 15l, 1.5, "hello"};
    auto visitor=overloaded {
            [](auto arg) { std::cout << arg << ' '; },
            [](double arg) { std::cout << std::fixed << arg << ' '; },
            [](const std::string& arg) { std::cout << std::quoted(arg) << ' '; }
        };

    for(auto& v: vec) {
     std::visit(visitor, v);
        }
   *@endcode
   *
   * @tparam Ts A deduced set of overloaded callable objects
   */
  template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
  //! explicit deduction guide (not needed as of C++20)
  template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;
}




#endif /* EXAMPLES_SRC_UTILITIES_OVERLOADED_HPP_ */
