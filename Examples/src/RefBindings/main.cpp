#include <iostream>
#include <memory>
#include <vector>


void fun(const std::vector<int> & a)
{
  std::cout<< "calling fun(const std::vector<int>&) "<<a.size()<<std::endl;

}

/*
void fun(std::vector<int> & a)
{
  std::cout<< "calling fun(std::vector<int>&) "<<a.size()<<std::endl;

}
*/
void fun(std::vector<int>&& a)
{
  auto c = std::move(a);
  std::cout<< "calling fun(std::vector<int>&&) "<<c.size()<<std::endl;
}


/*
void fun(const std::vector<int>&& a)
{
  auto c = std::move(a);
  std::cout<< "calling fun(const std::vector<int>&&) "<<c.size()<<std::endl;
}
*/
// Universal references!
template <typename V>
void forwardToFun(V&& v)
{
  std::cout<<" Calling fun(v) with forwarding reference\n";
  fun(std::forward<V>(v));
}



int main(){

  std::vector<int> a{10,20,30};
  const std::vector<int> b={10,20,30};
  // calling on an lvalue
  std::cout<<"fun(a)"<<std::endl;
  fun(a);
  std::cout<<" a size "<<a.size()<<std::endl;
  // calling on a const lvalue
  std::cout<<"fun(b)"<<std::endl;
  fun(b);
  std::cout<<" b size "<<b.size()<<std::endl;
  // calling on a rvalue
  std::cout<<"fun(move(a))"<<std::endl;
  fun(std::move(a));
  std::cout<<" a size "<<a.size()<<std::endl;
  // calling on a const rvalue
  std::cout<<"fun(move(b))"<<std::endl;
  fun(std::move(b));
  std::cout<<" b size "<<b.size()<<std::endl;

  // Now the forwarding reference part
  // restore a wich is now empty!
  a = b;
  // calling on a lvalue
  std::cout<<"forwardToFun(a)"<<std::endl;
  forwardToFun(a);
  std::cout<<" a size "<<a.size()<<std::endl;
  // calling on a rvalue
  std::cout<<"forwardToFun(std::move(a))"<<std::endl;
  forwardToFun(std::move(a));
  std::cout<<" a size "<<a.size()<<std::endl;
 // calling on a const lvalue
  std::cout<<"forwardToFun(b)"<<std::endl;
  forwardToFun(b);
  std::cout<<" b size "<<b.size()<<std::endl;
  // calling on a const rvalue
  std::cout<<"forwardToFun(std::move(b))"<<std::endl;
  forwardToFun(std::move(b));
  std::cout<<" b size "<<b.size()<<std::endl;
  
}
