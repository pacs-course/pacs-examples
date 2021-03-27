#ifndef _HH_COMPARESTRING_HH
#define _HH_COMPARESTRING_HH
#include<string>
//! Function that compares two strings according to policy.
//! Here the policy is given as template parameter, we assume that
//! it is default constructible and has a call operator that take 2 chars
//! and returns true if and only if they are considered equivalent
template <class P>
bool equal (const std::string & a, const std::string & b){
  P equal = P{};
  if(a.size()==b.size())
    {
    for(unsigned int i=0;i<a.size();++i)
      {
        if (!equal(a[i],b[i])) return false;
      }
    return true;
    }
  else
    {
      return false;
    }
}
//! We can use functors for the different policies
/*!
 * \tparam Policy The policy used to compare characters
 */
template <class Policy>
class CompareString
{
public:
  bool operator() (const std::string & a, const std::string & b)const
  {
    if(a.size()==b.size())
      {
        for(std::size_t i=0u; i<a.size(); ++i)
          {
            if (!equal(a[i],b[i])) return false;
          }
        return true;
      }
    // strings have same common characters but different size
    else return false;
  }
private:
  Policy equal;
};


#endif
