#include<map>
#include<vector>
#include <iostream>
#include <string>
#include <fstream>
using namespace std;
using vec=vector<double>;
//! A function taking vectors a and b and const references.

/*!
  It computes
  @f$ \mathbf{c}=\mathbf{c}+\mathbf{a}\sum_i b_i $@f

  This version will not compute what you expect if you do aliasing.
 */

void fun(vec const & a, vec const & b, vec & c)
{
  for (unsigned i=0; i<c.size();++i)
    for (unsigned k=0;k<b.size();++k)
      c[i]+=a[i]*b[k];
}

//! A function taking vectors a and b and const references.

/*!
  It computes
  @f$ \mathbf{c}=\mathbf{c}+\mathbf{a}\sum_i b_i $@f

  This version will compute what you expect if you do aliasing.
 */
void fun2(vec const & a, vec const & b, vec & c)
{
  for (unsigned i=0; i<c.size();++i)
    {auto tmp=a[i];
      for (unsigned k=0;k<b.size();++k)
      c[i]+=tmp*b[k];
    }
}

//! This function has a side effect, it modifies the argument passed as reference
double seven (double & x)
{
  x=7.0;
  return 7.0;
}

int main()
{
  vec a={1.,2.,3.};
  vec b={1.,2.,3.};
  vec c={1.,2.,3.};

  cout<<"fun(a,b,c)"<<endl;
  fun(a,b,c);
  cout<<"a= ";
  for (auto i : a) cout<<i<<" ";
  cout<<endl;
  cout<<"c= ";
  for (auto i : c) cout<<i<<" ";
  cout<<endl<<endl<<endl;

  cout<<"fun(a,b,a)"<<endl;
  fun(a,b,a);
  cout<<"a= ";
  for (auto i : a) cout<<i<<" ";
  cout<<endl<<endl<<endl;

  a={1.,2.,3.};
  cout<<"fun2(a,b,a)"<<endl;
  fun2(a,b,a);
  cout<<"a= ";
  for (auto i : a) cout<<i<<" ";
  cout<<endl;

  // Now a different (and more dangerous) type of aliasing

  double z{0};// a variable initialised by zero
  auto y = seven(z)+ 3*z; // which is the value returned by 3*z? 0 or 21?
  // The order of operation in the previous expression is undefined
  // the result may depend on the compiler and on the optimization level
  std::cout <<"y = "<<y<<std::endl;
  
}
