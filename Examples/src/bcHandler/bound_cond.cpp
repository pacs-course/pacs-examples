#include <sstream>
#include "bound_cond.hpp"
namespace FEM{
  
  //! Defined through a lambda (C++11 only)
  BCFun zerofun([](double const t, double const * coord){return 0.0;});

  //! Defined through a lambda (C++11 only)
  BCFun onefun( [](double const t, double const * coord){return 1.0;});
  
  BCName intToBCName(int i){
    std::ostringstream tmp;
    tmp<<i;
    return tmp.str();
  }
  
  void BCBase::set_Id(BCId const & id){
    this->name=id.name;
    this->type=id.type;
  }
  
  void BCBase::set_entities(std::vector<int> const & e)const {
    this->entities_.assign(e.begin(),e.end());
  }
  
  
  void BCBase::showMe(std::ostream & stream) const{
    stream<<"BC name= "<<this->name()<<" "
	  <<"Type="<<this->type()<<std::endl;
    stream<<"Associated entities:"<<std::endl;
    std::vector<int> const entities(this->entities());
    int count(0);
    for(std::vector<int>::const_iterator k=entities.begin();
	k<entities.end();++k){
      stream<<*k;
      if(++count%8==0)stream<<std::endl;
      else stream<<" ";
    }
    if(!(count%8==0))stream<<std::endl;
  }
}
