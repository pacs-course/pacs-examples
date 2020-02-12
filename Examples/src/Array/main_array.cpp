#include<array>
#include<iostream>
struct aggr
{
  double a;
  int b;
};
// Since C++17 aggregates can have base classes
#if __cplusplus >= 201703L
struct aggr2 : aggr
{
  int c;
};
// Since c++17 we can use structured bindings on arrays and on
// aggregates!
auto anAggregate()
{
  return aggr{3.1415,7};
}
auto anArray2()
{
  return std::array<int,2>{1,2};
}
#endif
//! Print arrays. This version takes the dimension as parameter 
template <class T, std::size_t N>
std::ostream & operator <<(std::ostream & out,std::array<T,N> const & myarray)
{
  out<<"Array dimension:"<< N<<"\n";
  out<<"Array content:  :"<<std::endl;
  for (auto i: myarray) out<<i<<" ";
  out<<std::endl;
  return out;
}

//! Prints ths size of an array. 
/*!
  It is only to show the capabilities of tuple_size!
  In this case the size is computed at compile time, dfferently
  than using size().
 */
template <class Array>
void printSize(const Array & a, std::ostream & out=std::cout)
{
  out<<"Array dimension:"<<std::tuple_size<Array>::value<<"\n";
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
  // C++14 style
#if __cplusplus > 201103L
  array<int,3> a{1,2,3}; //brace initialization (internal braces may be omitted in C++14)
  array<double,3> c={1.,2.,3.}; //brace-equal initialization (internal braces may be omitted)
  // But we can omit all internal brackets in C++14!
  array<aggr,3> bb{5.0,3, 6.3,4, -1.0,7};
#else
  // C++11 style
  array<int,3> a{{1,2,3}}; //brace initialization
  array<double,3> c={{1.,2.,3.}}; //brace-equal initialization (internal braces may be omitted)
  // Initialization of an array ofa aggregates
  array<aggr,3> bb{{{5.0,3}, {6.3,4}, {-1.0,7}}};
#endif
  array<double,4> d; // Default initialization
  cout<<"Array b:"<<b<<
    "Array a:"<<a<<
    "Array c:"<<c<<
    "Array d:"<<d; // Note that d is NOT initialised to zero!
  // Here we cannot omit the extra bracket in C++14
#if __cplusplus > 201103L
  array<aggr,3> aa={5.0,3, 6.3,4, -1.0,7};
#else
  array<aggr,3> aa={{{5.0,3}, {6.3,4}, {-1.0,7}}};
#endif
  // You can interrogate the size. array::size returns
  // a constexpr so it is resolved by the compiler!
  cout<<" Array bb size= "<<bb.size()<<endl;
  cout<<" Array aa size= "<<aa.size()<<endl;
  printSize(aa); // testing the function
  // I can pass the array to a function taking a pointer
  // using data()
  fun(c.data(),c.size());
  //
  // C++17 extension for derived aggregates
#if __cplusplus >= 201703L
  std::array<aggr2,3> ss{bb[0],10,bb[1],20,bb[2],30};
  cout<<" Array ss size= "<<ss.size()<<endl;
  auto& xa = ss[0];
  std::cout<<"first elements of ss:"<<xa.a<<" "<<xa.b<<" "<<xa.c<<std::endl;
  // structured bindings
  auto [a1,a2]=anAggregate();
  std::cout<<"The aggregate contains:"<<a1<<" "<<a2<<std::endl;
  auto [v1,v2]=anArray2();
  std::cout<<"The array contains:"<<v1<<" "<<v2<<std::endl;
#endif
}
