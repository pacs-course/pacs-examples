#include<iostream>
#include "class_ws.hpp"
#include<limits>
//! Initialization of static variable
double WS::sum(0.0);
int  WS::counter(0);

WS::WS(double value):myValue(value){
  ++WS::counter;
  WS::sum+=value;
};

WS::WS(const WS & w):myValue(w.myValue){
  ++WS::counter;
  WS::sum+=myValue;
};

WS & WS::operator =(WS const & w){
  WS::sum-=myValue;
  myValue=w.myValue;
  WS::sum+=myValue;
  return *this;
}

WS::~WS(){
  --WS::counter;
  WS::sum-=myValue;
};


double WS::average(){
  if(WS::counter==0)
    // quiet_NaN returns not a number without signalling a floating
    // point exception.
    return std::numeric_limits<double>::quiet_NaN();
  else
    return WS::sum/static_cast<double>(WS::counter);
}
   
int WS::howmany()
{return WS::counter;}

void WS::reset(){WS::sum=0.0;}

