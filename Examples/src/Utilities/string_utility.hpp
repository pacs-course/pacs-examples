#ifndef HHH_STRING_UTILITIES
#define HHH_STRING_UTILITIES
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>
/*
  Part of this software has been taken from internet. I thank the 
  original (unknwown) authors.
  It has been changed to make it more readable using some C++11 fetures. Even if may be less efficient
  than original code.
 */
namespace Utility{
  
  //! trim from left
  /**
     @param a A string
     @return the trimmed string
     @note a is also returned trimmed: after a call to 
     ltrim(a) string a is trimmed!! 
   */
  static inline std::string &ltrim(std::string &s) 
  {
    auto const & loc = std::locale();
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
				    [&loc]
				    (std::string::const_reference c)
				    {
				      return ! std::isspace(c,loc);
				    }
				    )
	    );
    return s;
  }
  
  
  //! trim from right
  /**
     @param a A string
     @return the trimmed string
     @note a is also returned trimmed: after a call to 
     ltrim(a) string a is trimmed!! 
   */
  static inline std::string &rtrim(std::string &s) {
    auto const & loc = std::locale();
    s.erase(std::find_if(s.rbegin(), s.rend(), 
			 [&loc]
			 (std::string::const_reference c)
			 {
			   return !std::isspace(c,loc);
			 }).base(),
	    s.end()
	    );
    return s;
  }
  
  //! trim from both ends
  /**
     @param a A string
     @return the trimmed string
     @note a is also returned trimmed: after a call to 
     ltrim(a) string a is trimmed!! 
   */
  static inline std::string &trim(std::string &s) {
    return ltrim(rtrim(s));
  }
  
  //! Converting a whole string using current locale
  /**
     @param s A string
     @return the modified string
     @note s is also returned trimmed: after a call to 
     toupper(a) string a is upper case!! 
   */

  /**@{*/
  std::string toupper(std::string & s);
  std::string tolower(std::string & s);
  /**@}*/
}
#endif
