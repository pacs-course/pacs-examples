#ifndef IS__CLONABLE__HPP
#define IS__CLONABLE__HPP
#include <type_traits>
#include <utility>
/*!An example of metaprogramming.

  It tests whether a class
  has the method clone(). I do not check the return type however
  (it can be done using std::is_same<> but is a little more involved).
*/

// A strange namespace name to avoid ambiguity with the version stored in
// CloningUtilities.hpp
namespace apsc::CloningTrait
{
//! A template that takes anything
/*!
  void_t<T> is always equal to void.  It is a way of
  checking that a type has been defined, irrespectively from the
  actual type.

  @note The type void is very special, it represent the "null type" Any type can
  be converted to void type, so it is the best way to express "I want a type but
  I don't care which one"
*/
#if __cplusplus >= 201703L
//! C++17 has void_t
using std::void_t;
#else
//! My own void_t
template <typename> using void_t = void;
#endif
/*!  Primary template for has_clone.

  The second argument is used to
  activate SFINAE.  By default it inherits from false_type, so it
  represent "false" Remember that std::false_type is convertible to
  a boolean equal to false.

  @tparam  T the type to test
  @tparam Sfinae A dummy template parameter used only for activating SFINAE. It
  should have a default  value but it is indifferent which one, void is fine.
  You don't need to name the parameter since it is not used in the body of the
  class. We could have written
   @code
   template<typename T, typename= void>
   @endcode
   but giving a name simplifies documentation.
 */
template <typename T, typename Sfinae = void>
struct has_clone : public std::false_type
{};

/*!  Specialized version that is activated if T is clonable.

  Indeed, if T is not clonable the second template parameter cannot
  be substituted with a valid type. So SFINAE applies and this
  version is discarded. If not, this specialized version will be used because
the second template parameter is now given, and specialized version have
precedence. Note that it inherits from std::true_type, so it is convertible to
true.

  declval<T&>() allows to test the return type of clone() with no need of
creating an object of type T. Moreover, we use T& because T may be a polymorphic
object, and clone() may be defined in the base class.

  @note I am not checking the return type of clone() only its existence It is
possible also to check the return type, but it becomes more involved. I might to
use std::enable_if<>. For instance to check whether clone() returns a
std::unique_ptr<Base> I can do

@code
  template<typename T>
  struct has_clone
  <
    class T, class Base,
    typename
    enable_if<std::is_convertible<decltype( std::declval<T&>().clone() ),
                           std::unique_prt<Base>
                           >::value
             >::type
  >: std::true_type {};
@endcode
*/
template <typename T>
struct has_clone<T, void_t<decltype(std::declval<T &>().clone())> >
  : std::true_type
{};

//! A helper function (maybe the only one the user will actually use!)
template <class T>
constexpr bool
isClonable()
{
  return has_clone<T>{};
}

template <typename  T>
constexpr bool has_clone_v=  has_clone<T>::value;

} // namespace apsc::CloningTrait
#endif
