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

// A function that takes a pointer to an array of double
// Old C and C++98 style

void fun(double const * a, int n)
{
  std::cout<<"Array dimension:"<< n<<"\n";
  std::cout<<"Array content:  :"<<std::endl;
  for (int i=0;i<n;++i) std::cout<<a[i]<<" ";
  std::cout<<std::endl;
}

int main()
{
  using namespace std;
  std::cout<<__cplusplus<<std::endl;
  array<int,3> b{{1,2,3}}; //brace initialization (array is an aggregate!!)
#if __cplusplus > 201103L
  array<int,3> a{1,2,3}; //brace initialization (internal braces may be omitted in C++14)
#else
  array<int,3> a{{1,2,3}}; //brace initialization (but not in C++11)
#endif
  array<double,3> c={1.,2.,3.}; //brace-equal initialization (internal braces may be omitted)
  array<double,4> d; // Default initialization
  cout<<b<<a<<c<<d; // Note that d is NOT initialised to zero!
  // Here we cannot omit the extra bracket 
  //  array<aggr,3> aa={{5.0,3}, {6.3,4}, {-1.0,7}}; ERROR
  array<aggr,3> aa={{{5.0,3}, {6.3,4}, {-1.0,7}}};
  // But we can omit all internal brackets!
  array<aggr,3> bb{5.0,3, 6.3,4, -1.0,7};
  // You can interrogate the size. array::size returns
  // a constexpr so it is resolved by the compiler!
  cout<<" Array size= "<<bb.size()<<endl;
  // I can pass the array to a function taking a pointer
  // using data()
  fun(c.data(),c.size());
}
