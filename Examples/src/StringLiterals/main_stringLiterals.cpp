#include <iostream>
#include <string>
#include <typeinfo>
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







}
