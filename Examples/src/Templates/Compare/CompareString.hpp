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
//! But we can use functors
/*!
 * \tparam Policy The policy used to compare characters
 */
template <class Policy>
struct CompareString
{
  bool operator() (const std::string & a, const std::string & b)const {
    if(a.size()==b.size())
      {
        for(std::size_t i=0u; i<a.size(); ++i)
          {
            if (!Policy::eq(a[i],b[i])) return false;
          }
        return true;
      }
    // strings have same common characters but different size
    else return false;
}
 /*
  *  Of course, if I want I can do a specialization for the case where I want the standard, case sensitive, comparision
  *  template<>
  *  struct CompareString<CaseCompare>
  *  {
  *    bool operator() (const std::string & a, const std::string & b)const
  *    {
  *       return a == b;
  *    }
  *  };
  */
};

#endif
