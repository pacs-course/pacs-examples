#include <iostream>
#include <vector>
#include <set>
#include <type_traits>


/* Naive solution: it does not work with overloaded member functions
template <class T>
struct has_push_back {
  template <class S> static std::true_type test(decltype(&S::push_back));
  template <class S> static std::false_type test(...);

  using type = decltype(test<T>(nullptr));
};
*/


struct none_t;


template <class...>
struct enable_if_valid {
  using type = none_t;
};

template <class... S>
using enable_if_valid_t = typename enable_if_valid<S...>::type;


namespace aux {
  template <class, class = none_t>
  struct default_push_back_signature {
    using type = none_t;
  };

  template <class T>
  struct default_push_back_signature<
      T
    , enable_if_valid_t<typename T::value_type>
  >
  {
    using value_type = typename T::value_type;
    using type = void(const value_type &);
  };
} // end namespace aux

// This avoid the ambiguous partial specialization, when user defined partial
// specialization are given.
template <class T>
struct default_push_back_signature
  : aux::default_push_back_signature<T> {};

template <class T>
using default_push_back_signature_t =
    typename default_push_back_signature<T>::type;


template <class T, class = default_push_back_signature_t<T> >
struct has_push_back /* {
  static_assert(..., "Invalid signature for push_back member function");
} */;

template <class T>
struct has_push_back<T, none_t>: std::false_type {};

template <class T, class Return, class... Args>
struct has_push_back<T, Return(Args...)> {
  template <class S>
  static typename std::is_same<
    Return, decltype(std::declval<S>().push_back(std::declval<Args>()...))
  >::type test(S *);

  template <class S>
  static std::false_type
  test(...);

  using type = decltype(test<T>(nullptr));
};

template <class T, class Signature = default_push_back_signature_t<T> >
using has_push_back_t = typename has_push_back<T, Signature>::type;    


template<typename T>
struct Test {
  using value_type = T;
  double push_back(const T &);
};


/* This enable an ambiguous partial specialization
namespace aux {
  template <class T>
  struct default_push_back_signature<std::vector<T> > {};
}
*/
template <class T>
struct default_push_back_signature<Test<T>> {
  using type = double(const T &);
};

#define HAS_MEMBER_TRAITS(NAME) \
namespace aux { \
  template <class T, class = none_t> \
  struct has_member_##NAME : std::false_type {}; \
  template <class T> \
  struct has_member_##NAME <T, enable_if_valid_t<typename T::NAME> >: std::true_type {}; \
} \
template <class T> using has_member_##NAME = typename aux::has_member_##NAME <T>::type; \
template <class T> using has_member_##NAME##_t = typename has_member_##NAME<T>::type;

HAS_MEMBER_TRAITS(value_type)


int main() {
  std::cout << std::boolalpha;

  std::cout << has_push_back_t<std::vector<int>>::value << std::endl;
  std::cout << has_push_back_t<std::set<int>>::value << std::endl;
  // Without partial specialization (97--100) this evalueates false!
  std::cout << has_push_back_t<Test<int>>::value << std::endl;

  std::cout << has_member_value_type<int>::value << std::endl;
  std::cout << has_member_value_type<std::vector<int>>::value << std::endl;

  return 0;
}