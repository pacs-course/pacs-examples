#include "nonLinSys.hpp"

namespace NonLinearSystems{
  NonLinSys::returnType
  NonLinSys::operator()(argumentType const & x) const{
    returnType tmp(M_funs.size());
    unsigned int j=0;
    for (auto i: M_funs)tmp[j++]=i(x);
    return tmp;
  }

  NonLinSys::returnType
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

  double
  NonLinSys::norm(argumentType const & x) const
  {
    auto tmp=this->operator()(x);
    return tmp.norm();
  }








}// end of namespace
