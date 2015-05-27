#include<iostream>
void foo(int & a){
  std::cout<<"calling void foo(int & a)"<<std::endl;
}
void foo(const int & a){
  std::cout<<"calling void foo(const int & a)"<<std::endl;
}
int createFive() {
  int tmp{5};
  return tmp;
}
void goo(int & a)
{
  std::cout<<"calling void goo(int & a)"<<std::endl;
}

void goo(int && a)
{
  std::cout<<"calling void goo(int && a)"<<std::endl;
}

void goo(const int & a)
{
  std::cout<<"calling void goo(const int & a)"<<std::endl;
}


int main()
{
  int a{5};
  int & b{a};
  foo(25);            //foo(const int & a)  
  foo(a);             //foo(int & a)        
  foo(b);             //foo(int & a)        
  foo(createFive());  //foo(const int & a)  
  goo(25);            //goo(int && a) NOTE!      
  goo(a);             //goo(int & a)        
  goo(createFive());  //goo(int && a) NOTE!       
  goo(b);             //goo(int & a)       

  //int& c=createFive(); // ERROR! Cannot bind a temporary to a non-const reference
  const int& c=createFive(); //Ok lifetime of temporary is increased!
  foo(c);  //foo(const int & a)
  goo(c);  //goo(const int & a)
  std::cout<<c<<std::endl;
}
