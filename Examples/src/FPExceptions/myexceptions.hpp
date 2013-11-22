#include "myexceptions.hpp"
#include <sstream>

namespace{
  std::string bMessage(double s){
    std::ostringstream tmp;
    tmp<<"Small divisor. Value= "<< s<< "smaller than min value="<<SmallDivisor::minValue;
    return tmp.str();
  }
}

SmallDivisor::SmallDivisor(double s):BadDivision(bMessage(s)){}

double SmallDivisor::minValue=1.e-30;

