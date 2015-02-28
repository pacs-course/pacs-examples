#include "string_utility.hpp"
std::string Utility::toupper(std::string & s)
{
  // convert to upper case.`
  std::transform(s.begin(), 
		 s.end(),
		 s.begin(), 
		 [](std::string::const_reference c)
		 {
		   return std::toupper(c,std::locale());
		 } 
		 );
  return s;
}

std::string Utility::tolower(std::string & s)
{
  // convert to upper case.`
  std::transform(s.begin(), 
		 s.end(),
		 s.begin(), 
		 [](std::string::const_reference c)
		 {
		   return std::tolower(c,std::locale());
		 } 
		 );
  return s;
}
