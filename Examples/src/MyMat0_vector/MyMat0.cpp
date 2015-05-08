#include <cstdlib>
#include <cmath>
#include <ctime>
#include "MyMat0.hpp"
namespace LinearAlgebra {
  
  MyMat0::MyMat0(size_type n, size_type m, StoragePolicySwitch sPolicy):
    nr(n), nc(m), data(n*m),myPolicy(sPolicy)
  {
    switch (this->myPolicy)
      {
      case ROWMAJOR:
	getIndex = &MyMat0::rowMajorPolicy;
	break;
      case COLUMNMAJOR:
	getIndex = &MyMat0::columnMajorPolicy;
	break;
      }
    // initialize to zero
    for(size_type i=0;i<nc*nr;++i) data[i]=0.0;
  }
  
  
  size_type MyMat0::rowMajorPolicy(size_type const & i, 
				   size_type const & j) const{
    return j + i*nc;
    }

  size_type MyMat0::columnMajorPolicy(size_type const & i, 
				      size_type const & j) const{
      return i + j*nr;
  }

  double  MyMat0::getValue(size_type const i, size_type const j) const
  {
    // todo : this test should be hidden in a private method
    if  (i>=nr || i<=nc)
      {
	// todo this way of handling errors could be bettered
	// using exceptions
	std::cerr<<" Out of bounds in MyMat0";
	std::exit(1);
      }
    return this->operator ()(i,j);
  }
  
  
  void  MyMat0::setValue(size_type const i, size_type const j, double const & v)
  {
    if  (i>=nr || i<=nc)
      {
	std::cerr<<" Out of bounds";
	std::exit(1);
      }
    this->operator ()(i,j)=v;
  }
  
  
  void MyMat0::resize(size_type const n, size_type const m)
  {
    if(n*m != nc*nr)
      {
	// clear data storage
	data.clear();
	// in c++11 we could use move semantic
	std::vector<double>().swap(data);
	if(n*m != 0)
	  {
	    data.resize(n*m);
	    //! Set to zero
	    for (size_type i =0;i<n*m;++i)data[i]=0.0;
	  }
      }
    //! fix number of rows and column
    nr=n;
    nc=m;
  }
  
  double MyMat0::normInf() const{
    double vmax(0.0);
    if(nr*nc==0)return 0;
    
    for (size_type i=0;i<nr;++i){
      double vsum=0;
      for (size_type j=0;j<nc;++j) vsum+=std::abs(this->operator()(i,j));
      vmax=std::max(vsum,vmax);
    }
    return vmax;
  }
  
  double MyMat0::norm1() const{
    if(nr*nc==0)return 0;
    double vmax(0);
    for (size_type j=0;j<nc;++j){
      double vsum=0;
      for (size_type i=0;i<nr;++i) vsum+=std::abs(this->operator()(i,j));
      vmax=std::max(vsum,vmax);
    }
    return vmax;
  }
  
  double MyMat0::normF() const{
    if(nr*nc==0)return 0;
    double vsum=0;
    for (size_type i=0;i<nr*nc;++i) vsum+=data[i]*data[i];
    vsum=std::sqrt(vsum);
    return vsum;
  }
  
  void MyMat0::vecMultiply(const std::vector<double> &v, std::vector<double> & res) const
  {
    if(v.size() != nc){
      std::cerr<<" Vector must have the right size"<<std::endl;
      std::exit(1);
    }
    res.resize(nr,0);
    // ! for efficiency I use two different algorithms
    switch(this->myPolicy)
      {
      case ROWMAJOR:
	// Classic A*v row by row
	for (size_type i=0;i<nc*nr;++i) res[i / nc] += data[i]*v[i % nc];
	break;
      case COLUMNMAJOR:
	// result is a linear combination of the columns of A
	for (size_type i=0;i<nc*nr;++i) res[i % nr] += data[i]*v[i / nr];
	break;
      }
  }
  
  void  MyMat0:: fillRandom(unsigned int seed)
  {
    if (seed==0) seed=std::time(0);
    double rmax=double(RAND_MAX+2.0);
    std::srand(seed);
    if(nr*nc>0)
      for (size_type i=0;i<nc*nr;++i) data[i]=double(std::rand()+1)/rmax;
  }
  
  
  void MyMat0::showMe(std::ostream & out) const{
    out<<"[";
    for (size_type i=0;i<nr;++i){
      for (size_type j=0;j<nc-1;++j) out<< this->operator()(i,j)<<", ";
      out<< this->operator()(i,nc-1);
      if(i==nr-1)
	out<<"]"<<std::endl;
      else
	out<<std::endl;
    }
  }
  
  std::vector<double> operator * (MyMat0 const & m,std::vector<double> const & v){
    std::vector<double> tmp;
    m.vecMultiply(v,tmp);
    return tmp;
  }

 } // End of Namespace



