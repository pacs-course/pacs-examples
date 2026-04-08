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
 * @brief Primary template.
 *
 * Defaults to false.
 *
 * @note The name is kept for backward compatibility, although the trait checks
 * whether T is an instantiation of Template.
 * @tparam T Type to test
 * @tparam Template Primary class template to match against
 */
template <class T, template <class...> class Template>
struct is_specialization : std::false_type
{};
/*!
 * @brief Partial specialization matching Template<Args...>.
 *
 * This specialization is selected when the tested type is an instantiation of
 * the same primary class template.
 *
 * @par Usage
 * @code
 * // true
 * static_assert(is_specialization<std::vector<int>, std::vector>{}, "");
 * // false
 * static_assert(!is_specialization<std::vector<int>, std::list>{}, "");
 * @endcode
 *
 * @tparam Template Primary class template
 * @tparam Args Template arguments used in the instantiation
 *
 */
template <template <class...> class Template, class... Args>
struct is_specialization<Template<Args...>, Template> : std::true_type
{};
/*!
 * @brief Convenience variable template for is_specialization.
 *
 * is_specialization_v<S, T> is true when S is an instantiation of T.
 *
 * This is the utility most users should prefer, since std::remove_cvref_t
 * strips cv-qualifiers and references before evaluating the trait.
 */
template <class T, template <class...> class Template>
inline constexpr bool is_specialization_v =
  is_specialization<std::remove_cvref_t<T>, Template>::value;
/*!
 * @brief Concept matching instantiations of a given class template.
 *
 * @par Usage
 * @code
 * template <class T>
 * requires Specialization_of<T, std::vector>
 * Foo{};
 *
 * Foo<std::vector<int>> foo; // fine
 * Foo<std::list<int>> foo;   // fails
 *
 * @endcode
 *
 * @tparam T Type to test
 * @tparam Template Primary class template to match against
 *
 */
template <class T, template <class...> class Template>
concept Specialization_of = is_specialization_v<T, Template>;

} // namespace apsc::TypeTraits

#endif /* UTILITIES_IS_SPECIALIZATION_HPP_ */
