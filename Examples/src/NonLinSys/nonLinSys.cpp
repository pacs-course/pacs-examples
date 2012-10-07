#include "nonLinSys.hpp"

namespace NonLinearSystems{
  returnType
  NonLinSys::operator()(argumentType const & x) const{
    returnType tmp(M_funs.size());
    unsigned int j=0;
    for (auto i: M_funs)tmp[j++]=i(x);
    return tmp;
}

returnType
NonLinSys::residual(argumentType const & x) const{
  returnType tmp(M_funs.size());
  unsigned int j=0;
  for (auto i: M_funs)tmp[j++]=-i(x);
  return tmp;
}

void
NonLinSys::addToSystem(funType const & f){
  M_funs.push_back(f);
}

unsigned int
NonLinSys::numEq()const{
  return M_funs.size();
}

double
NonLinSys::norm(argumentType const & x) const
{
  auto tmp=this->operator()(x);
  return tmp.norm();
}








}// end of namespace
