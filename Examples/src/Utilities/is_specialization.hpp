/*
 * is_specialization.hpp
 *
 *  Created on: Apr 15, 2023
 *      Author: forma
 */

#ifndef UTILITIES_IS_SPECIALIZATION_HPP_
#define UTILITIES_IS_SPECIALIZATION_HPP_
#include <type_traits>
namespace apsc::TypeTraits
{
  /*!
   * Primary template
   *
   * Defaults to false
   *
   * @note The term "specialized" here is misplace. It should be called is_instance_of
   * @todo change the name. It's misleading. It does not really tests specializations but instances
   * @tparam T The class we want to check if it is a specialization
   * @tparam Template The unspecialized type
   */
  template <class T, template<class...> class Template>
  struct is_specialization : std::false_type {};
/*!
 * Checks if a class template is a specialization of a primary
 *
 * Usage:
 * @code
 * // true
 * static_assert(is_specialization<std::vector<int>, std::vector>{}, "");
 * // false
 * static_assert(!is_specialization<std::vector<int>, std::list>{}, "");
 * @endcode
 *
 * @tparam Template<Args..> The class we want to check is is a specialization
 * @tparam Template The unspecialised class
 *
 */
  template <template <class...> class Template, class... Args>
  struct is_specialization<Template<Args...>, Template> : std::true_type {};
  /*!
   * The values of is_specialization
   *
   * is_specialization_v<S,T>{} is equivalent to true if S is a template, specialization of T
   *
   * This is the utility the user should use, since std::decay_t will take away possoble const and references
   * form the Args.
   */
  template <class T, template<class...> class Template>
   inline constexpr bool is_specialization_v=is_specialization<std::decay_t<T>,Template>::value;
  /*!
   * Concept that tests specialization
   *
   * Usage
   * @code
   * template <class T>
   * requires Specialization_of<T,std::vector>
   * Foo{};
   *
   * Foo<std::vector<int>> foo // fine
   * Foo<std::list<int>> foo // fails
   *
   * @endcode
   *
   */
  template <class T, template<class...> class Template>
  concept Specialization_of=is_specialization_v<T,Template>;


}




#endif /* UTILITIES_IS_SPECIALIZATION_HPP_ */
