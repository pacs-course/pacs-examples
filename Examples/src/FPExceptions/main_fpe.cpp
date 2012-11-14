#include <iostream>
#include <cmath>
#include <string>
#include "trapfpe.hpp"
#include "rational.hpp"
double f1(double);
double f2(double);
int main(){
  using namespace std;
  bool theEnd=false;
  double a,b;
  cout<<"Type 1 if you want to trap FPEs ";
  int ans;
  cin>>ans;
  bool on=(ans==1);
  if (on) cout<< "FPE TRAP ACTIVATED"<<endl;
  else cout<< "FPE TRAP NOT ACTIVATED"<<endl;
  do{
    std::cout<<"Gimme 2 numbers a and b"<<std::endl<<
      "I will compute 1/a and log(b)"<<std::endl;
    std::cin>>a>>b;
    try{
      double z=f1(a);
      std::cout<<" 1/"<<a<<" is "<<z<<std::endl;
      std::cout.flush();
      test_fpe_exception(on);
      double x=f2(b);
      std::cout<<" log("<<b<<") is "<<x<<std::endl;
      std::cout.flush();
      test_fpe_exception(on);
    }
    catch (BadFPOper & x){
      std::cout<<x.what()<<std::endl;
    }
    catch (BadDivision & x){
      std::cout<<x.what()<<std::endl;
    }
    catch (...){
      std::cout<<"Another (unknown) exception occurred"<<std::endl;
    }
    
    std::cout<<" Give me a rational, first numerator and then denomintor"<<std::endl;
    int n,d;
    std::cin>>n>>d;
    try{
      Rational r(n,d);
    }
    catch (ZeroDivision & re){
      cout<<re.what()<<endl;
      feclearexcept(FE_INVALID |FE_DIVBYZERO|FE_OVERFLOW);
    }
    cout<<"Type yes is you want to continue: ";
    string answer;
    cin>>answer;
    theEnd=answer.find("yes")==string::npos;
  }while(!theEnd);
}

  double f1(double a){
    return 1./a;
  }
  
  double f2(double c){
    return std::log(c);
  }
  
