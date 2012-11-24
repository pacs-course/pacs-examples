#include <sstream>
#include "bound_cond.hpp"
namespace FEM{
using std::string;
using std::ostringstream;
  
  double zerofun(double const, double const *){
    return 0.0;
  }
  
  BCName intToBCName(int i){
    ostringstream tmp;
    tmp<<i;
    return tmp.str();
  }
  
  BCId::BCId(BCType t, int n):type(t),name(intToBCName(n)){}
  
  //! Make sure that the ordering is "type first"
  bool operator < (BCId const & l, BCId const & r){
    if (l.type == r.type)return l.name< r.name;
    else return l.type < r.type;
  }
  
  bool operator == (BCId const & l, BCId const & r){
    return l.type == r.type &&  l.name == r.name;
  }
  void BCBase::set_entities(std::vector<int> const & e)const {
    /*
      this->_entities.resize(e.size());
      std::vector<int>::const_iterator j=e.begin();
      for(std::vector<int>::iterator i=this->_entities.begin();
      i<this->_entities.end();++i,++j)*i=*j; */
    this->_entities.assign(e.begin(),e.end());
  }
  
  bool compareOnType(BCBase const & l, BCBase const & r){
    return l.type() < r.type();
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
