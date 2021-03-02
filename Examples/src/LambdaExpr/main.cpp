/*
 * main.cpp
 *
 *  Created on: Mar 2, 2021
 *      Author: forma
 *  Example of lambda expressions
 */
#include <iostream>
#include <vector>
#include <cmath>
/*!
 * A class to show the use of lambda expressions the uses the
 * object of the class
 */
class UseLambda
{
public:
  /*!
   * A function the uses the this pointer
   * @param y a value
   * @return a double
   */
  auto fun(double y) const
  {
    // x is this->x
    // being fun() a const method this is passes as a const UseLambda*
    // I can use the data of the object read-only
    auto f=[this](double z){return x*std::sin(z);};
    return f(2*y);
  }
  /*!
   * This function takes this by value
   * In this case inside the lambda I have a copy of myself
   * In this example it is not of great use, but there are situations where
   * you may want the copy of the object and not a pointer to it
   * @param y
   * @return
   */
  auto fun2(double y) const
  {
    auto f=[*this](double z){return x*std::cos(z);};
    return f(2*y);
  }
  void set_x(double z){x=z;}
private:
  double x=0.;
};

/*!
 * A function that returns a lambda expression
 * @param x a value
 * @return A lambda that computes y -> x*sin(y)
 */
auto csin(double x)
{
  // x is copied in the lambda
  return [x] (double y){return x*std::sin(y);};
}

 int main()
 {
   UseLambda useLambda;
   std::cout<<useLambda.fun(5.0)<<" "<<useLambda.fun2(5.0)<<std::endl;
   // now I change the state
   useLambda.set_x(-3.5);
   std::cout<<useLambda.fun(5.0)<<" "<<useLambda.fun2(5.0)<<std::endl;

   // Now some other use of lambda expressions

   double z =3.5;
   // passing by value and by reference: the main difference
   // here the z in the scope of the lambda ia a copy of z
   auto l1 = [z](const double & y){return z*y;};
   // here the z in the scope of the lambda ia a reference (an alias) of z
   auto l2 = [&z](const double & y){return z*y;};

   // constexpr are automatically included in a lambda expession!
   // and a lambda expression may use another lambda expression!
   constexpr double oneThird=1./3.;
   // I am capturing l1
   auto l3 = [&l1](double const & y){return oneThird*l1(y);};
   std::cout<<"l3(9)"<<l3(9.)<<std::endl;



   std::cout<<"l1(5) and l2(5)"<<l1(5)<<" "<<l2(5)<<std::endl;
   std::cout<<" Now I change z\n";
   z=-3.5;
   // The value of the z local to l1 is unchanged. It is a copy of z at the moment of the cration
   // of the lambda expression. While l2 stores a reference ... therefore:
   std::cout<<"l1(5) and l2(5)"<<l1(5)<<" "<<l2(5)<<std::endl;
   // Using a function returning a lambda
   auto f1 = csin(4.0); // get the lambda 4*sin
   double w = f1(2.0); //w = 4*sin(2)
   std::cout<<" w="<<w<<" is equal to 4*sin(2)="<<4.*std::sin(2.)<<std::endl;

 }




