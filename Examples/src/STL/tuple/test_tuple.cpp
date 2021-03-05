/* The following code example is taken from the book
 * "The C++ Standard Library - A Tutorial and Reference, 2nd Edition"
 * by Nicolai M. Josuttis, Addison-Wesley, 2012
 *
 * (C) Copyright Nicolai M. Josuttis 2012.
 * Permission to copy, use, modify, sell and distribute this software
 * is granted provided this copyright notice appears in all copies.
 * This software is provided "as is" without express or implied
 * warranty, and with no claim as to its suitability for any purpose.
 */
#include <tuple>
#include <iostream>
#include <complex>
#include <string>
#include <set>
#include <unordered_set>

#include "HashTuple.hpp"
#include "printtuple.hpp"
using namespace std;
// A function returning a tuple
auto fun(double const & a)
{
  return std::make_tuple("Values: ",a*a,a*a*a);
};

int main()
{
  // create a four-element tuple
  // - elements are initialized with default value (0 for fundamental types)
  tuple<string,int,int,complex<double>> t{"paolo",1,2,{1,1}};
  std::cout<< "t= "<<t<<std::endl;
  
  // create and initialize a tuple explicitly
  tuple<int,float,string> t1{41,6.3,"nico"};
  
  // "iterate" over elements:
  cout << get<0>(t1) << " ";
  cout << get<1>(t1) << " ";
  cout << get<2>(t1) << " ";
  cout << endl;
  // Use the streaming operator to print
  cout<<" Tuple t1:"<<t1<<std::endl;

  
  // create tuple with make_tuple()
  // - auto declares t2 with type of right-hand side
  // - thus, type of t2 is tuple<int, float, char[]>
  // Note the suffix f to indicate that 44 is a float (and not a double)
  auto t2 = make_tuple(22,44.0f,"nico");
  cout<<" Tuple t2:"<<t2<<std::endl;

  
  // assign second value in t2 to t1
  get<1>(t1) = get<1>(t2); 
  cout<<" Tuple t2 after change:"<<t2<<std::endl;
  
  // comparison and assignment
  // - including type conversion from tuple<int,int,const char*>
  //   to tuple<int,float,string>
  if (t1 < t2) {  // compares value for value
    t1 = t2;    // OK, assigns value for value
  }
  // SInce 41 is not smaller than 22 and < does lexicografic comparison
  // t1 should be unchanged

  cout<<" Tuple t1 after comparison:"<<t1<<std::endl;

  // Now tie.. tie ties a tuple (ora a pair!) to references
  // and you can use the magic of std::ignore to ignore parts
  float x;
  int i;
  std::tie(i,x,std::ignore)=t1;
  std::cout<<"i= "<<i<<" x= "<<x<<std::endl;
  // In C++17 you can use structured bindings
  auto [message,xSquare,xCube]= fun(3.0);
  std::cout<< message<<" "<<xSquare<<" "<<xCube<<std::endl;

  // You can craate set of tuples as long the type of the contained elements are all
  // comparable, i.e. operator < is valid on them
  using MyTuple =std::tuple<int,double,std::string>;
  std::set<MyTuple> st;
  st.insert({3,5.6,"Anna"});
  st.insert({4,2.6,"Giorgio"});
  st.insert({-1,7.6,"Mike"});
  st.insert({9,5.10,"Elise"});
  std::cout<<"Tuples in the set\n";
for (auto v:st) std::cout<<v<<std::endl;
// I can also use unordered sets with the utility hashTuple

std::unordered_set<MyTuple,apsc::Utility::HashTuple<MyTuple> > ust;
// fill with the same values
ust.insert(st.begin(),st.end());
std::cout<<"Tuples in the unordered\n";
for (auto v:ust) std::cout<<v<<std::endl;






}
