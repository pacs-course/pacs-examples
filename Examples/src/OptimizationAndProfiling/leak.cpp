/*! Memory leak.

An example which shows how it is dangerous to have functions which
returns pointers (or references) to an object created in the function.
This example wants to point a common error. Look at the comments
in the code!

A GENERAL RULE: Functions should always return objects. You can then
eventually use move semantic to optimize assigment of the return
object. At most you may return references/pointers to object passed as
arguments to the function (but always ask yoursef: is it necessary? A
part when defining operators normally the answer to this question is
NO). 

Remember that "Resource acquisition is initialization!": what creates
a resource should also be responsible of its destruction.  If you need
to return a pointer (factories) return a "smart pointer" (which is in
fact an object that takes responsability of the owned resource).

 */
#include <iostream>
#include <vector>
#include <algorithm>
//! creates an array containing the fibonacci sequence
int * fibonacci(unsigned int const n){
  if (n==0) return nullptr; // C++11
  // where is the delete[]??
  int * tmp = new int[n];
  tmp[0]=1;
  if (n>1){
    tmp[1]=2;
    for (unsigned int i=2;i<n;++i) tmp[i]=tmp[i-1]+tmp[i-2];
  }

  return tmp;
}

//! creates a vector containing the fibonacci sequence
void
createFibonacci(std::vector<int> & v,unsigned int n){
  int * f=fibonacci(n); // use other function
  v.resize(n);
  // I use copy. Note the use of pointer arithmetics
  std::copy(f,f+n,v.begin());
  // Who is deleting f????
  delete [] f;
} 
  

int main ()
{
  constexpr unsigned int n=10; // C++11
  std::vector<int> fib;
  createFibonacci(fib,n);
  std::cout<<n<<"th fibonacci number= ";
  std::cout<<fib.back()<<std::endl;
}

