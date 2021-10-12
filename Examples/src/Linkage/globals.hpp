#ifndef HH_GLOBALS_HH
#define HH_GLOBALS_HH
//! \file globals.hpp Defines use of variables with external linkage

/*
 *  Global variables must be declared in a header file and makerd with the
 * extern keyword. With the use of the inline keyword, instead, they may be also
 * defined in the header file. A global variable can be set outside of all
 * scopes, in the so called global scope, but it is safer to use a namespace. If
 * you give a significant name to the namespace, it will also help you remind
 * that they are global variables indeed! A global variable in a name space is
 * also called a namespace variable.
 */
namespace Globals
{
//! A global (namespace) variable with external linkage. Only declared!
/*!
  It must be defined in a source file. It would have been better to create a
  namespace (see below)
 */
extern int globalValue;
//! You may also define a global variable in a header file using the inline
//! keyword and you can also initialize it

inline int anotherGlobalValue = 6;

//! a constexpr global should be defined in the header file

constexpr double Pi = 3.1415;

//! Declaration of a stuct that may represent global parameters of a program
struct GlobalParameters
{
  bool   negate;
  double g2;
};

//! define and initialize  global variable of type Global Parameters
inline GlobalParameters globalParameters2{true, Pi};

//! a can also use extern, but then it is only a declaration. The definition
//! goes in a source file
extern GlobalParameters globalParameters;
} // end namespace Globals
#endif
