#ifndef _HH_POLICIES_HH__
#define _HH_POLICIES_HH__
#include <cctype> // for toupper()
#include <algorithm>
//! Normal compare (case sensitive)
class CaseCompare {
public:  // I rely on the existing operator
  static bool eq(char const & a, char const & b)
  { return a==b;}
};

//! Case insensitive compare (case sensitive)
/*! 
  It works on char * and std::string
  Note, I am using the "vintage" form of std::toupper
  that works only with C-locale characters (i.e. ascii characters)
  If you want something able to operate on different character sets
  you have to use the version of toupper in <locale>, which is slightly
  more complex but can handle any locale character set.
  To do it, include <locale> and theplace the line with

  \code
  return std::toupper(a,std::locale())==std::toupper(b,std::locale());
  \endcode

*/
class NoCaseCompare {
public:
  static bool eq(char const  & a, char const & b)
  { return std::toupper(a)==std::toupper(b);}
};

//! The hash function for noCaseCompare. If I want to use an unordered associative container.

struct HashNoCase
{
    std::size_t operator()(std::string const& s) const noexcept
    {
        std::string nocomp{s};
        // convert to upper case
        for (auto & c : nocomp) c=std::toupper(c);
        // use system hash for strings
        return std::hash<std::string>{}(nocomp);
    }
};
#endif
