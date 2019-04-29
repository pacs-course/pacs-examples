#include <iostream>
class Foo
{
  // In this case the synthetic methods would be fine!
public:
  Foo()
  {
    std::cout<<" Calling default constructor"<<std::endl;
  }
  Foo(const Foo & f):M_x{f.M_x}
  {
    std::cout<<" Calling copy constructor"<<std::endl;
  }
                   
  Foo(const Foo&& f):M_x{std::move(f.M_x)}
  {
    std::cout<<" Calling move constructor"<<std::endl;
  }

  Foo & operator =(const Foo & f)
  {
    std::cout<<" Calling copy assignement"<<std::endl;
    M_x=f.M_x;
    return *this;
  }

  Foo & operator =(Foo&& f)
  {
    std::cout<<" Calling move assignement"<<std::endl;
    M_x=std::move(f.M_x);
    return *this;
  }
  
  // in class initialization of member variable
  double M_x{0.0};
};

void f(Foo const & x)
{
  std::cout<<"Calling f(Foo const &)"<<std::endl;
}

void f(Foo& x)
{
  std::cout<<"Calling f(Foo&)"<<std::endl;
}

void f(Foo&& x)
{
  std::cout<<"Calling f(Foo&&)"<<std::endl;
}

Foo createFoo()
{
  return Foo{};
}

int main()
{
  std::cout<<"Creating a Foo object"<<std::endl;
  Foo foo1;
  std::cout<<"\nCopying the Foo object"<<std::endl;
  Foo foo2(foo1);
  std::cout<<"\nMoving the Foo object (foo1 is not available any more)"<<std::endl;
  Foo foo3(std::move(foo1));
  // auto x = foo1.M_x; // This can give inconsistent results or segfault!
  std::cout<<"\nAssigning the Foo object"<<std::endl;
  foo2 = foo3;
  std::cout<<"\nCalling foo3=createFoo()"<<std::endl;
  foo3=createFoo();
  std::cout<<"\nCalling Foo foo4(createFoo())"<<std::endl;
  Foo foo4(createFoo());
  std::cout<<" Note that here default constructor is called, not move constructor\n";
  std::cout<<" RVO (copy elision) optimization has been appllied!"<<std::endl;
  std::cout<<"\nCalling Foo foo5=std::move(foo4)"<<std::endl;
  Foo foo5=std::move(foo4);
  std::cout<<" Move constructor is called: it is an inizialization NOT an assignement"<<std::endl;
  std::cout<< "**************** Now function bindings "<<std::endl;
  std::cout<< " Creating a constant Foo object called cFoo\n";
  const Foo cfoo;
  std::cout<< " f(cfoo)\n";
  f(cfoo);
  std::cout<< " f(foo5):\n";
  f(foo5);
  std::cout<< " f(std::move(foo5)):\n";
  f(std::move(foo5));
  std::cout<< " f(createFoo()):\n";
  f(createFoo());
 }

