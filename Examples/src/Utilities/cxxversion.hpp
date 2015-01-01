#ifndef __CXXVERSION_HPP__
#define __CXXVERSION_HPP__
/*! A simple utility to determine the version of c++ you are using at compilation time
 */
#ifndef __cplusplus
#error A C++ compiler is required!
#endif 

namespace Utilities
{
#if __cplusplus >= 201103L
  //! True if you are using c++11
  constexpr bool is_cxx11()
  {
    return bool(__cplusplus == 201103L);
  }
  //! True if you are using c++14
  constexpr bool is_cxx14()
  {
    return bool(__cplusplus > 201103L);
  }
  //! True if you are using c++98
  constexpr bool is_cxx98()
  {
    return bool(__cplusplus == 199711L);
  }
#else
  //! True if you are using c++11
  inline bool is_cxx11()
  {
    return false;
  }
  //! True if you are using c++14
  inline bool is_cxx14()
  {
    return false;
  }
  //! True if you are using c++98
  inline bool is_cxx98()
  {
    return bool(__cplusplus == 199711L);
  }
#endif
}

#endif
