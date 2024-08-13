#include <iostream>
#include <locale>
#include <string>
#include <string_view> // since c++17
#include <typeinfo>

/*! @ brief Function to check if 'text' starts with 'prefix'
 *
 * This function is a simple example of the use of string_view
 *  @param text the string to be checked
 *  @param prefix the prefix to be checked
 *  @return true if 'text' starts with 'prefix', false otherwise
 */
bool
starts_with(std::string_view text, std::string_view prefix)
{
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

  // Note that typeid returns two different types. The type name is mangled so
  // not easily recognizable, but it is certainly different!
  auto c = "This is a C-style null terminated string!";
  std::cout << c << std::endl;
  std::cout << "The type of c is " << typeid(c).name() << std::endl;
  // If you uncomment the next line you get a compilation error.
  // a C-style string has no method size() (indeed  has no methods at all! is
  // just a char*)
  // std::cout<<"of length "<<c.size()<<std::endl;

  auto s = "This is a C++ string!"s;
  std::cout << s << std::endl;
  std::cout << "The type of s is " << typeid(s).name() << std::endl;
  // Here we have size since s is a std::string
  std::cout << "of length " << s.size() << std::endl;

  // Using string_view you can opearate on string literals without copying them
  // and operate both on C++ and C-style strings without problems
  // You can operate also on substrings of a string_view with no additional
  // copies Note that we are mixing c-style strings and c++ strings: string_view
  // is a bridge Note: you have a lot of utilities to manipulate strings, even
  // regular expressions!
  std::cout << "Does '" << c << "' start with 'This'?=" << std::boolalpha
            << starts_with(c, "This") << std::endl;
  std::cout << "Does '" << s << "' start with 'This'?=" << std::boolalpha
            << starts_with(s, "This") << std::endl;
  std::cout << "Does '" << trunk << "' start with 'This'?=" << std::boolalpha
            << starts_with(trunk, "This") << std::endl;

  // And now a little experiment (try to understand why)
  std::cout << "The size of the character e is" << sizeof('e') << std::endl;
  std::cout << "The size of the character  è is" << sizeof('è') << std::endl;
  std::cout << "Why??\n";
  // I store e and è in a char
  char e_normal{'e'};
  // char e_accented{'è'}; // this is a multibyte character I get a compiler
  // error becouse of narrowing
  char e_accented =
    'è'; // Here I have implicit convertion from multibyte (int) to char
  std::cout << "Printing the characters e and è stored in a char: " << e_normal
            << " " << e_accented << std::endl;
  using namespace std::string_literals;
  std::string e_accented_str =
    "è"s; // the s not needed I can realy to implicit conversion
  std::string e_normal_str = "e"s;
  std::cout << "Printing the size of a c++ string storing e and è= "
            << e_normal_str.size() << " and " << e_accented_str.size()
            << std::endl;
  std::cout << "Printing è stored in a std::string: ";
  std::cout << e_accented_str[0] << "," << e_accented_str[1] << std::endl;
  // Handling multybyte character strings is not easy
  // I can use a wide string but the following code is not working on my PC
  // Better use std::string and not single characters.

  std::u8string utf8_str = u8"è";           // this is a utf-8 string
  wchar_t       e_u8accented = utf8_str[0]; // here I extract the character è
  // that I store in a wchar_t (4 bytes long).
  std::wcout << L"Printing the character è stored in a wchar_t: "
             << e_u8accented << std::endl; // but still it does not work
  return 0;
}
