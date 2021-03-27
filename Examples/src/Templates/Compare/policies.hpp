#ifndef _HH_POLICIES_HH__
#define _HH_POLICIES_HH__
#include <cctype> // for toupper()
#include <algorithm>
//! Normal compare (case sensitive)
struct CaseCompare {
  //! I rely on the existing operator
  bool operator()(char const & a, char const & b) const
  { return a==b;}
};

//! Case insensitive compare (case sensitive)
/*! 
  Note, Here am using the "vintage" form of std::toupper
  that works only with C-locale characters (i.e. ascii characters)
  If you want something able to operate on different character sets
  you have to use the version of toupper in <locale>, which is slightly
  more complex but can handle any locale character set.
  To do it, include <locale> and theplace the line with

  \code
  return std::toupper(a,std::locale())==std::toupper(b,std::locale());
  \endcode

However, in that case maybe you need to operate correctly with the character type
of your character encoding locale. I leave this to the interested readers, here I am
assuming that we are using the usual C-locale (the characters of the US keyboard)
*/
struct NoCaseCompare {
public:
  //! equality operator that ignore case of characters
  bool operator()(char const  & a, char const & b) const
  { return std::toupper(a)==std::toupper(b);}
};




//! The hash function for string for case insensitive comparison.
/*!
 * I need to define it if I want to use an unordered associative container.
 * It will be passed as hash function. It is implemented-in-term-of the standard
 * hash for strings.
 */

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
