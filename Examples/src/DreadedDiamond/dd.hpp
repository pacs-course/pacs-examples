#ifndef _HH_DD_HH__
#define _HH_DD_HH__
class A{
public:
  A(double data=0):my_data(data){}
  double getData(){return my_data;}
  virtual double run()const =0;
protected:
  double my_data;
};

//class B: public A{ // WRONG
class B: virtual public A{
public:
  B(double data=0):A(data){}
  virtual double run()const {return 3*this->my_data;}  
};

//class C: public A{ //Wrong
class C: virtual public A{
public:
  C(double data=0):A(data){}
  virtual double run()const{return 4*this->my_data;}  
};

class D: public C, public B{
public:
  //! COnstructor
  D(double data=0):A(data){}
  //! Local version of run().
  /*!
    It is implemented in function of the run() methods of B and C.
   */
  virtual double run()const{
    // I need the full qualified name 
    // to disambiguate the calls
    return B::run()*C::run();
  }
  //! Set the internal data
  void setMydata(double x){this->my_data=x;}
};
#endif
