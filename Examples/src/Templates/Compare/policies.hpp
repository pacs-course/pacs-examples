#ifndef _HH_POLICIES_HH__
#define _HH_POLICIES_HH__
#include <cctype> // for toupper()
//! Normal compare (case sensitive)
class Ncomp {
public:  // I rely on the existing operator
  static bool eq(char const & a, char const & b)
  { return a==b;}
};

//! Case insensitive compare (case sensitive)
/*! 
  It works on char * and std::strings
*/
class Nocase {
public:
  static bool eq(char const  & a, char const & b)
  { return std::toupper(a)==std::toupper(b);}
};

#endif
