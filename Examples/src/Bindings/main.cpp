#include<iostream>
void foo(int& a){
  std::cout<<"using void foo(int& a)"<<std::endl;
}
void foo(const int& a){
  std::cout<<"using void foo(const int& a)"<<std::endl;
}
int createFive() {
  int tmp{5};
  return tmp;
}
void goo(int& a)
{
  std::cout<<"using void goo(int& a)"<<std::endl;
}

void goo(int&& a)
{
  std::cout<<"using void goo(int&& a)"<<std::endl;
}

void goo(const int& a)
{
  std::cout<<"using void goo(const int& a)"<<std::endl;
}


int main()
{
  int a{5};
  int & b{a};
  std::cout<<"calling foo(25) "<<std::endl;
  foo(25);            //foo(const int& a)  
  std::cout<<"calling foo(a) "<<std::endl;
  foo(a);             //foo(int& a)        
  std::cout<<"calling foo(b) "<<std::endl;
  foo(b);             //foo(int& a)        
  std::cout<<"calling foo(createFive()) "<<std::endl;
  foo(createFive());  //foo(const int& a)  
  std::cout<<"calling goo(25) "<<std::endl;
  goo(25);            //goo(int&& a) NOTE!      
  std::cout<<"calling goo(a) "<<std::endl;
  goo(a);             //goo(int& a)        
  std::cout<<"calling goo(createFive()) "<<std::endl;
  goo(createFive());  //goo(int&& a) NOTE!
  std::cout<<"calling goo(b) "<<std::endl;
  goo(b);             //goo(int & a)       

  //int& c=createFive(); // ERROR! Cannot bind a temporary to a non-const reference
  const int& c=createFive(); //Ok lifetime of temporary is increased!
  std::cout<<"calling foo(c) "<<std::endl;
  foo(c);  //foo(const int & a)
  std::cout<<"calling goo(c) "<<std::endl;
  goo(c);  //goo(const int & a)
}
