#include<iostream>
#include<vector>
#include<typeinfo>
void foo(int& a){
  std::cout<<"using void foo(int&)"<<std::endl;
}
void foo(const int& a){
  std::cout<<"using void foo(const int&)"<<std::endl;
}
int createFive() {
  int tmp{5};
  return tmp;
}
void goo(int& a)
{
  std::cout<<"using void goo(int&)"<<std::endl;
}

void goo(int&& a)
{
  std::cout<<"using void goo(int&&)"<<std::endl;
}

void goo(const int& a)
{
  std::cout<<"using void goo(const int&)"<<std::endl;
}

using Vector=std::vector<double>; // to save typing

// Now the interesting stuff, an anticipation of move semantic
/*
// Standard way, comment if you want to use the the std::forward magic
void gvect(const Vector& v)
{
  std::cout<<"using void gvect(const T&), v size is "<<v.size()<<std::endl;
}

void gvect(Vector&& x)
{
  Vector v(std::move(x));
  std::cout<<"using void gvect(T&&), v size is "<<v.size()<<std::endl;
}
*/
// Now the std:;forward magic: all in one!
template<class T>
void  gvect(T&& x)
{
  T v(std::forward<T>(x));
  std::cout<<"using void gvect<T>(T&&) with std::forward" <<
    " created vector size is "<<v.size()<<" Input vector size is now "<<x.size()<< std::endl;
}


int main()
{
  int a{5};
  int & b{a};
  //int& c=createFive(); // ERROR! Cannot bind a temporary to a non-const reference
  const int& c=createFive(); //Ok lifetime of temporary is increased!
  constexpr int dcx=2;
  std::cout<<"calling foo(25) "<<std::endl;
  foo(25);            //foo(const int& a)  
  std::cout<<"calling foo(a) (a is int)"<<std::endl;
  foo(a);             //foo(int& a)        
  std::cout<<"calling foo(b) (b is int&)"<<std::endl;
  foo(b);             //foo(int& a)        
  std::cout<<"calling foo(createFive()) "<<std::endl;
  foo(createFive());  //foo(const int& a)  
  std::cout<<"calling foo(c) (c is const int &)"<<std::endl;
  foo(c);  //foo(const int & a)
  std::cout<<"calling foo(dcx) (dcx is a constexpr)"<<std::endl;
  foo(dcx);  //foo(const int & a)
  std::cout<<"calling goo(25)"<<std::endl;
  goo(25);            //goo(int&& a) NOTE!      
  std::cout<<"calling goo(a) (a is int)"<<std::endl;
  goo(a);             //goo(int& a)        
  std::cout<<"calling goo(b) (b is int&)"<<std::endl;
  goo(b);             //goo(int & )       
  std::cout<<"calling goo(createFive()) "<<std::endl;
  goo(createFive());  //goo(int&& ) NOTE!
  std::cout<<"calling goo(c) (c is const int &)"<<std::endl;
  goo(c);  //goo(const int & a)
  std::cout<<"calling goo(dcx) (dcx is a constexpr)"<<std::endl;
  goo(dcx);  //goo(const int & a)

  std::cout<<"\n\n NOW SOMETHING MORE INTERESTING\n\n";
  Vector v(100,3.0); // a vector containing 3 of size 100

  std::cout<<"calling gvect(v) (v is a vector)"<<std::endl;
  gvect(v);
  std::cout<< "The size of v after the call is: "<<v.size()<<std::endl;

  std::cout<<"calling gvect(std::move(v)) (v is a Vector)"<<std::endl;
  gvect(std::move(v));
  std::cout<< "The size of v after the call is: "<<v.size()<<std::endl;

  std::cout<<"I pass a vector created on the fly: gvect(vector<double>(10))"<<std::endl;
  gvect(std::vector<double>(10));
}
