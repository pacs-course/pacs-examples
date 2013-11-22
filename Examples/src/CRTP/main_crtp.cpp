#include "crtp.hpp"
int main(){
  Base<Derived> a;
  a.fun(5.0);
  Base<Derived>::fstatic();
  Base<Derived2> a2;
  a2.fun(5.0);
}
