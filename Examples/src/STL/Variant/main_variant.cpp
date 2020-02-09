#include<variant>
#include <iostream>
#include<string>
#include<vector>
#include <type_traits>
//! A function returning eiter a string or an int
//! \param i according to which we return one or the other
std::variant<std::string,int>
fun(int const & value)
{
  if (value>0)
    return value;
  else
    return std::string("Non positive input");
}
//! A callable function that operates differently on dfferent types
struct Operate
{
  //! operator for string
  void operator()(std::string const & x){std::cout<<x<<std::endl;};
  //! operator for int
  void operator()(int const & x){std::cout<<10*x<<std::endl;}
};


int main()
{
  std::cout<<"Type an integer"<<std::endl;
  int i;
  std::cin>>i;
  auto result=fun(i);
  // Interrogate the actual type stored
  // and take corresponding action
  // to retrive value you gan use std::get<T>()
  // where T is either an integer with the position in the variant
  // or a type when there is no ambiguity, like now
  if(std::holds_alternative<std::string>(result))
    std::cout<<" I have a string saying: "<<std::get<std::string>(result)<<std::endl;
  else
    std::cout<<" I have a int equal to : "<<std::get<int>(result)<<std::endl;
  // Now an example of visit : a container with variant
  std::vector<std::variant<std::string,int>> v{1,2,"hello","world"};
  // now visit each variant
  // The correct overloaded call operator is called on the variant
  for (auto i : v) std::visit(Operate(),i);
}
