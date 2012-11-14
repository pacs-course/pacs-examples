#ifndef __MYEXCEPTIONS_HPP__
#define __MYEXCEPTIONS_HPP__
#include <exception>
#include <stdexcept>
#include <string>
//! General overflow
class BadFPOper: public std::runtime_error { 
public:
  BadFPOper(std::string const & s="Bad Floating point operation"):
    runtime_error(s){}
  virtual ~BadFPOper(){}
};

//! Floating point overflow
class FloatOverflow: public BadFPOper{
public:
  FloatOverflow(std::string const s="Floating Point Overflow"):BadFPOper(s){}
  virtual ~FloatOverflow(){}
};   

//! Floating point overflow
class InvalidFPOperation: public BadFPOper {
public:
  InvalidFPOperation(std::string const s="Invalid FP Operation"):BadFPOper(s){}
  virtual ~InvalidFPOperation(){}
};   

//Bad division
class BadDivision: public std::runtime_error { 
public:
  BadDivision(std::string const & s):runtime_error(s){};
  virtual ~BadDivision(){}
};

//! Zero division

class ZeroDivision: public BadDivision {
public:
  ZeroDivision(std::string const & s="Divide by zero"):
    BadDivision(s){}
  virtual ~ZeroDivision(){}
};   

//! Small divisor
class SmallDivisor: public BadDivision { 
public:
  SmallDivisor(std::string const & s="Small Divisor"):BadDivision(s){};
  SmallDivisor(double s);
  virtual ~SmallDivisor(){}
  static double minValue;
};
#endif
