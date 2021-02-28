#ifndef HH_TOSTRING__HH
#define HH_TOSTRING__HH
#include<sstream>
/*! @brief A simple utility the converts to sting.

  The boost library lexical_cast provides a very extensive set of tool to cast
  expressions to a sting object. Here the scope is more limited. This utility
  produces a string from anything for which the streaming operator 
  has been defined.

  @param input An expression for which the << operator is defined
  @param flag std::ios_base::fmtflags for formatting
  @return the sting produced by <<input 

*/
namespace Utilities
{
  template<typename T>
  std::string toString(
		       T const & input, 
		       std::ios_base::fmtflags flag)
  {
    std::stringstream stream;
    stream.flags(flag);// set formatting flags
    stream<<input;
    return stream.str();
  }

  //! Version without specifying flags
  template<typename T>
  std::string toString(
		       T const & input)
  {
    std::stringstream stream;
    stream<<input;
    return stream.str();
  }
}
#endif
