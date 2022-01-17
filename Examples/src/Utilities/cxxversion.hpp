#ifndef __CXXVERSION_HPP__
#define __CXXVERSION_HPP__
/*! A simple utility to determine the version of c++ you are using at
 * compilation time
 */
#ifndef __cplusplus
#error A C++ compiler is required!
#endif

namespace Utilities
{
#if __cplusplus >= 201103L
// I am using at least c++11 so I can use constexpr functions
//! True if you are using c++11
constexpr bool
is_cxx11()
{
  return (__cplusplus == 201103L);
}
//! True if you are using c++14
constexpr bool
is_cxx14()
{
  return (__cplusplus == 201402L);
}
//! True if you are using c++20
constexpr bool
is_cxx20()
{
  return (__cplusplus == 202002L);
}
//! True if you are using c++17
constexpr bool
is_cxx17()
{
  return (__cplusplus == 201703L);
}
//! True if you are using c++98
// Only to avoid compilation errors
constexpr bool
is_cxx98()
{
  return false;
}
//! True if you are using at least c++11
constexpr bool
is_atleast_cxx11()
{
  return true;
}
//! True if you are using at least c++14
constexpr bool
is_atleast_cxx14()
{
  return (__cplusplus >= 201402L);
}
//! True if you are using at least c++17
constexpr bool
is_atleast_cxx17()
{
  return (__cplusplus >= 201703L);
}
//! True if you are using at least c++20
constexpr bool
is_atleast_cxx20()
{
  return (__cplusplus >= 202002L);
}
//! True if you are using at least c++98
// Only to avoid compilation errors
constexpr bool
is_atleast_cxx98()
{
  return true;
}
#else
//! True if you are using c++11
inline bool
is_cxx11()
{
  return false;
}
//! True if you are using c++14
inline bool
is_cxx14()
{
  return false;
}
//! True if you are using c++98
inline bool
is_cxx98()
{
  return (__cplusplus == 199711L);
}
//! True if you are using c++17
inline bool
is_cxx17()
{
  return false;
}

//! True if you are using c++20
inline bool
is_cxx20()
{
  return false;
}

//! True if you are using at least c++11
inline bool
is_atleast_cxx11()
{
  return false;
}
//! True if you are using at least c++14
inline bool
is_atleast_cxx14()
{
  return false;
}
//! True if you are using at least c++98
inline bool
is_atleast_cxx98()
{
  return true;
}
//! True if you are using at least c++17
inline bool
is_atleast_cxx17()
{
  return false;
}

//! True if you are using at least c++20
inline bool
is_atleast_cxx20()
{
  return false;
}

#endif
} // namespace Utilities

#endif
