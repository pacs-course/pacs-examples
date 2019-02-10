#include <sstream>
#include "bound_cond.hpp"
#include <map>
namespace FEM{
  

  //! Defined through a lambda (C++11 only)
  BCFun zerofun([](double const , double const *){return 0.0;});

  //! Defined through a lambda (C++11 only)
  BCFun onefun([](double const, double const * ){return 1.0;});
  
  BCType stringToBCType(BCName const & s){
    // an use of static function variables
    // Maybe would be better to make it a method of bcBase and store the map as a
    // member of the class
    static std::map<BCName,BCType> dict {
      {"Dirichlet",Dirichlet},
      {"Neumann", Neumann},
      {"Robin", Robin},
      {"Other",Other},
    };
    auto res = dict.find(s);
    if  (res!=dict.end())
      return res->second;
    else
      return None;
  }
  
  double BCBase::apply(double const t, double const * coord) const {
      return M_fun(t,coord);
    }
  
  std::ostream & BCBase::showMe(std::ostream & stream) const{
    stream<<"BC name= "<<this->name()<<" "
	  <<"Type="<<this->type()<<std::endl;
    stream<<"Associated entities:"<<std::endl;
    auto const entities=this->entities();
    int count(0);
    for(auto k : entities){
      stream<<k;
      if(++count%8==0)stream<<std::endl;
      else stream<<" ";
    }
    if(!(count%8==0))stream<<std::endl;
    return stream;
  }
}
