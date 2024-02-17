#include <iostream>
#include <string>
#include <typeinfo>
#include <string_view> // since c++17

/*! @ brief Function to check if 'text' starts with 'prefix'
  * 
  * This function is a simple example of the use of string_view
  *  @param text the string to be checked
  *  @param prefix the prefix to be checked
  *  @return true if 'text' starts with 'prefix', false otherwise
  */
bool starts_with(std::string_view text, std::string_view prefix) {
    return text.substr(0, prefix.size()) == prefix;
}



int
main()
{


  // Example of use of raw strings
  std::string interpreted = "Hello\nWorld";
  std::string notInterpreted = R"!(Hello\nWorld)!";

  std::cout << "interpreting \\n as newline:" << std::endl;
  std::cout << interpreted << std::endl;
  std::cout << "treating \\n verbatim:" << std::endl;
  std::cout << notInterpreted << std::endl;

  // Now something surprising... \0 is the termination

  std::string trunk = "Hello\0World";
  std::string noTrunk = R"!(Hello\0World)!";
  std::cout << "interpreting end of string  \\0:" << std::endl;
  std::cout << trunk << std::endl;
  std::cout << "treating end of string  \\0 verbatim:" << std::endl;
  std::cout << noTrunk << std::endl;

  // string literals (here I explain only s)
  using namespace std::string_literals;

  // Note that typeid returns two different types. The type name is mangled so not
  // easily recognizable, but it is certainly different!
  auto c = "This is a C-style null terminated string!";
  std::cout<<c<<std::endl;
  std::cout<<"The type of c is "<<typeid(c).name()<<std::endl;
  // If you uncomment the next line you get a compilation error.
  // a C-style string has no method size() (indeed  has no methods at all! is just a char*)
  //std::cout<<"of length "<<c.size()<<std::endl;

  auto s = "This is a C++ string!"s;
  std::cout<<s<<std::endl;
  std::cout<<"The type of s is "<<typeid(s).name()<<std::endl;
  // Here we have size since s is a std::string
  std::cout<<"of length "<<s.size()<<std::endl;

  // Using string_view you can opearate on string literals without copying them
  // and operate both on C++ and C-style strings without problems
  // You can operate also on substrings of a string_view with no additional copies
  // Note that we are mixing c-style strings and c++ strings: string_view is a bridge
  std::cout<< "Deas '"<<c<<"' start with 'This'?="<<std::boolalpha<<starts_with(c,"This")<<std::endl;
  std::cout<< "Deas '"<<s<<"' start with 'This'?="<<std::boolalpha<<starts_with(s,"This")<<std::endl;
  std::cout<< "Deas '"<<trunk<<"' start with 'This'?="<<std::boolalpha<<starts_with(trunk,"This")<<std::endl;

}
