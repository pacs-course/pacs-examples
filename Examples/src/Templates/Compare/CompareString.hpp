#ifndef _HH_COMPARESTRING_HH
#define _HH_COMPARESTRING_HH
#include<string>
//! Compares two strings according to policy.
template <class P>
bool equal (const std::string & a, const std::string & b){
  if(a.size()==b.size()){
    for(unsigned int i=0;i<a.size();++i)
      if (!P::eq(a[i],b[i])) return false;
    return true;}
  else return false;
}
#endif
