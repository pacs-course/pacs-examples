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

    @note, since c++17 the standard library provides it, with the type
    trait std::void_t. See documentation.
  */
  template<typename> struct Void { using type=void; };
  
  /*!  Primary template for has_clone.  

    The second argument is used to
    activate SFINAE.  By default it inherits from false_type, so it
    represent "false" Remember that std::false_type is convertible to
    a boolean equal to false

    @tpar  T the type to test
    @tpar Sfinae A dummy template parameter used only for activating SFINAE. It should have a default  value
                 but it is indifferent which one, void is fine.
   */
  template<typename T, typename Sfinae = void>
  struct has_clone: public std::false_type {};
  
  /*!  Specialised version that is activated if T is clonable.
    
    Indeed, if T is not clonable the second template parameter cannot
    be substituted with a valid type. So SFINAE applies and this
    version is discarded. If not, this specialised version will be referred because the second template parameter
    is now given, and specialised version have precedence to the primary template.
    Note that it inherits from std::true_type, so it is
    convertible to true.
    
    declval<T&>() allows to test the return type of clone() (the
    result of the test is irrelevant in this case) with no need of
    creating an object of type T. Moreover, we use T& because T may be
    a polymorphic object, and clone() may be defined in the base
    class.

    @note I am not checking the return type of clone() only its existence
    It is possible also to check the return type, but it becomes more involved
    I need to use enable_if. For instance to check whether clone() returns
    a std::unique_ptr<Base> I can do

    template<typename T>
    struct has_clone
    <
      T,
      typename
      enable_if<std::is_same<decltype( std::declval<T&>().clone() ),
                             std::unique_prt<Base>
                             >::value
               >::type
    >: std::true_type {};


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
