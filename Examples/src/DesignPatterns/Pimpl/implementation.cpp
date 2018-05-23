#include "implementation.hpp"
namespace internal
{ 
  double implementation::get_a()const {return this->M_a;}
  double & implementation::get_a(){return this->M_a;}
  int implementation::get_b()const {return this->M_b;}
  int & implementation::get_b(){return this->M_b;}
  std::array<double,2>   implementation::get_A()const {return this->A;}
  std::array<double,2> & implementation::get_A(){return this->A;}
}// end internal
