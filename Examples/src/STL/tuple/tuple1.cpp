/* The following code example is taken from the book
 * "C++ Templates - The Complete Guide"
 * by David Vandevoorde and Nicolai M. Josuttis, Addison-Wesley, 2002
 *
 * (C) Copyright David Vandevoorde and Nicolai M. Josuttis 2002.
 * Permission to copy, use, modify, sell and distribute this software
 * is granted provided this copyright notice appears in all copies.
 * This software is provided "as is" without express or implied
 * warranty, and with no claim as to its suitability for any purpose.
 */
#include <tuple>
#include <iostream>

int main()
{
  // create and use tuple with only one field (not very useful)
  std::tuple<int> t1(2);
  std::get<0>(t1) += 42;
  std::cout << std::get<0>(t1)<< std::endl;
  
  // create and use triplet
  std::tuple<bool,int,double> t3;

  t3=std::make_tuple(true,2,5.0);

  std::cout << std::get<0>(t3) << ", ";
  std::cout << std::get<1>(t3) << ", ";
  std::cout << std::get<2>(t3) <<std::endl;

  // Concatenating tuples
  auto t4 =std::tuple_cat(t1,t3);
  
  std::cout << std::get<0>(t4) << ", ";
  std::cout << std::get<1>(t4) << ", ";
  std::cout << std::get<2>(t4) << ", ";
  std::cout << std::get<3>(t4) <<std::endl;

  // extracting values from tuple into variables
  // using tie. I ignore the first component
  int i;
  double b;
  std::tie(std::ignore,i,b)=t3;

  std::cout<<"i= "<<i<<", b="<<b<<std::endl;

  // You can compare tuples, if the type of all elements is
  // comparable. And the comparison is lexycographic

  auto t_a = std::make_tuple(1,2,3);
  auto t_b = std::make_tuple(3,4,5);

  std::cout.setf(std::ios::boolalpha);
  std::cout<< "is t_a< t_b ? "<< (t_a<t_b)<<std::endl;

  // You can swap tuples

  std::swap(t_a,t_b);
  std::cout<< "is t_a< t_b ? "<< (t_a<t_b)<<std::endl;
  
  
}

