#include<variant>
#include <iostream>
#include<string>
#include<vector>
#include <type_traits>
//! A function returning either a string or an int
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
  void operator()(std::string const & x)const {std::cout<<x<<std::endl;};
  //! operator for int
  void operator()(int const & x)const {std::cout<<10*x<<std::endl;}
};

// This part is only for the nerds of you (skip if you are not interested)
// This is a class with which you can overload an arbitrary number of functors
template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
// explicit deduction guide (not explained at lecture: it allows to create an overloaded object just using
// the constructor. Very nice, but it was too much for the course....). Moreover it is not necessary since C+=20.
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;
// End nerdish part


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

  // nerdish part: I construct the visitor using overloaded
  std::cout<<" Nerdish part"<<std::endl;
  overloaded visitor{
      [](int i){std::cout<<"it's an integer equal to "<<i<<std::endl;},
          [](const std::string & i){std::cout<<"it's a string equal to "<<i<<std::endl;}};
  for (auto i : v) std::visit(visitor,i);
  // end nerdish section

}

