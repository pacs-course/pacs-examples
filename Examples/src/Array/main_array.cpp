#include<array>
#include<iostream>
struct aggr
{
  double a;
  int b;
};
// print arrays
template <class T, std::size_t N>
std::ostream & operator <<(std::ostream & out,std::array<T,N> const & myarray)
{
  out<<"Array dimension:"<< N<<"\n";
  out<<"Array content:  :"<<std::endl;
  for (auto i: myarray) out<<i<<" ";
  out<<std::endl;
  return out;
}

// Getting the size of an Array (of course when it is not obvious)
template<class Array>
std::size_t getArraySize(Array const &)
{
  return std::tuple_size<Array>::value;
}

int main()
{
  using namespace std;
  array<int,3> b{{1,2,3}}; //brace initialization (array is an aggregate!!)
#if __cplusplus > 201103L
  array<int,3> a{1,2,3}; //brace initialization (internal braces may be omitted in C++14)
#else
  array<int,3> a{{1,2,3}}; //brace initialization (but not in C++11)
#endif
  array<double,3> c={1.,2.,3.}; //brace-equal initialization (internal braces may be omitted)
  array<double,4> d; // Default initialization
  cout<<b<<a<<c<<d;
  // Here we cannot omit the extra bracket (unless we use C++14)
  //array<aggr,3> aa={{5.0,3}, {6.3,4}, {-1.0,7}}; COMPILE ERROR
  array<aggr,3> aa={{{5.0,3}, {6.3,4}, {-1.0,7}}};
  // But we can omit all intenral brackets!
  array<aggr,3> bb{5.0,3, 6.3,4, -1.0,7};
  cout<<" Array size= "<<getArraySize(bb)<<endl;
}
