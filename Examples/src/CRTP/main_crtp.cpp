#include "crtp.hpp"
int main(){
  Base<Derived> a;
  a.fun(5.0);
  Base<Derived>::fstatic();
}
