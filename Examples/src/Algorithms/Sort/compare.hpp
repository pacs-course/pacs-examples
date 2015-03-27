#include <cctype> // for toupper()
#include <locale> // for toupper(std::locale)
#include <iostream>
#include <string> // for string
#include <algorithm> // for sort() and min()

//! Example of using a comparison operator as policy
/*
  We use the sort function of the standard library which
  may take a function object as third argument.
 */

//! This class does the normal comparison, case sensitive.
/*!
  It is here only for example, since there is no need to use it as
  case sensitive sort is the default behaviour.
 */
class Ncomp {
public:
  bool operator () (std::string const & a, std::string const & b){
    return a< b;}
};

//! Compare by ignoring case.
/*! We use the toupper function provided by <cctype> The new C+11
  standard provides a new toupper() function which support
  internationalization, i.e. font encoding different form ascii.
 */
class Nocase {                                 // compare by ignoring case 
public:
  bool operator () (std::string const & a, std::string const & b)
  {
    // Lexycografic comparison
    for(decltype(a.size()) i=0;i<std::min(a.size(),b.size()); ++i){
      if (std::toupper(a[i])!= std::toupper(b[i])) 
	return std::toupper(a[i])<std::toupper(b[i]);
    }
    return (a.size()<b.size());
  }
};

class Nocase_locale {// compare by ignoring case (using locale 
public:
  Nocase_locale(std::locale const & loc=std::locale()):M_loc(loc){};
  bool operator () (std::string const & a, std::string const & b)
  {
    // Lexycografic comparison
    for(decltype(a.size()) i=0;i<std::min(a.size(),b.size()); ++i){
      if (std::toupper(a[i],M_loc)!= std::toupper(b[i],M_loc)) 
	return std::toupper(a[i],M_loc)<std::toupper(b[i],M_loc);
    }
    return (a.size()<b.size());
  }
private:
  std::locale M_loc;
};
