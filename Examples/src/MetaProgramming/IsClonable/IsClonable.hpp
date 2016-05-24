#ifndef IS__CLONABLE__HPP
#define IS__CLONABLE__HPP
#include <utility>
#include <type_traits>
/*!An example of metaprogramming. 
  
  It tests whether a class
  has the method clone(). I do not check the return type however
  (it can be done using std::is_same<> but is a little more involved).
*/
namespace TypeTraits
{
  //! A template that takes anything
  /*!  
    Void<T>::type is always equal to void.  It is used as a way of
    checking that a type has been defined, irrespectively from the
    actual type.
  */
  template<typename> struct Void { using type=void; };
  
  /*!  Primary template for has_clone.  

    The second argument is used to
    activate SFINAE.  By default it inherits from false_type, so it
    represent "false" Remember that std::false_type is convertible to
    a boolean equal to false.
   */
  template<typename T, typename Sfinae = void>
  struct has_clone: std::false_type {};
  
  /*!  Specialised version that is activated if T is clonable.
    
    Indeed, if T is not clonable the second template parameter cannot
    be substituted with a valid type. So SFINAE applies and this
    version is discarded. Note that it inherits from std::true_type.
    
    declval<T&>() allows to test the return type of clone() (the
    result of the test is irrelevant in this case) with no need of
    creating an object of type T. Moreover, we use T& because T may be
    a polymorphic object, and clone() may be defined in the base
    class.
  */
  template<typename T>
  struct has_clone<
    T, 
    typename Void<
      decltype( std::declval<T&>().clone() )
      >::type
    >: std::true_type {};

  //! A helper function (maybe the only one the user will actually use!)
  template <class T>
  constexpr bool isClonable() { return has_clone<T>(); }

}
#endif
