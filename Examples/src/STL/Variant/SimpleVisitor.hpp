//
// Created by forma on 28/02/23.
//

#ifndef EXAMPLES_SIMPLEVISITOR_HPP
#define EXAMPLES_SIMPLEVISITOR_HPP
#include <string>
#include <complex>
#include <variant>
#include <iostream>
namespace apsc
{
//! A variant type tht can contain different things...
  using Variant1=std::variant<int,double,std::string,std::complex<double>>;

  //! A visitor for my Variant1
  /*!
   * It provides an overladed set of call operators, each one
   * matching one of the possoble VAriant1 types
   */
 struct SimpleVisitor
 {
   void
   operator()(int i) const
   {
     std::cout << "int: " << i << "\n";
   }
   void
   operator()(double f) const
   {
     std::cout << "double: " << f << "\n";
   }
   void
   operator()(const std::string &s) const
   {
     std::cout << "string: " << s << "\n";
   }
   void
   operator()(const std::complex<double> &c) const
   {
     std::cout << "complex: " << c.real() << "+" << c.imag() << "i\n";
   }
 };
/*!
 * This overload of the steaming operator is only to show some features of a variant
 *
 * It is inline because I want to avoid having a source file
 * @param out output stream
 * @param v A Variant1
 * @return  The sam output stream
 */
    inline std::ostream & operator<< (std::ostream & out, Variant1 const & v)
    {
     if (std::holds_alternative<int>(v))
        std::cout << "the variant holds an int!\n";
    else if (std::holds_alternative<double>(v))
        std::cout << "the variant holds a double\n";
    else if (std::holds_alternative<std::string>(v))
        std::cout << "the variant holds a string\n";
    else if (std::holds_alternative<std::complex<double>>(v))
        std::cout << "the variant holds a complex\n";

    return out;
    }


}
#endif // EXAMPLES_SIMPLEVISITOR_HPP
