#include<iostream>
#include "class_ws.hpp"
#include<limits>
//! Initialization of static variable
double WS::sum(0.0);
int  WS::counter(0);

WS::WS(double const & value):myValue(value){
  ++WS::counter;
  WS::sum+=value;
};

WS::WS(const WS & w):myValue(w.myValue){
  ++WS::counter;
  WS::sum+=myValue;
};

WS::WS(WS&& w):myValue(std::move(w.myValue)){
  ++WS::counter;
  WS::sum+=myValue;
};


WS & WS::operator =(WS const & w){
  WS::sum-=myValue;
  myValue=w.myValue;
  WS::sum+=myValue;
  return *this;
}

WS & WS::operator =(WS&& w){
  WS::sum-=myValue;
  myValue=std::move(w.myValue);
  WS::sum+=myValue;
  return *this;
}

WS::~WS(){
  --WS::counter;
  WS::sum-=myValue;
};


double WS::average(){
  return WS::counter==0?
    0.0:
    WS::sum/static_cast<double>(WS::counter);
}
   
int WS::howmany()
{return WS::counter;}

void WS::reset(){WS::sum=0.0;}

