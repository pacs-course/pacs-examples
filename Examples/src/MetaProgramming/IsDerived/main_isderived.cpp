/*
 * main_isclone.cpp
 *
 *  Created on: Nov 16, 2008
 *      Author: forma
 */

#include "isDerived.hpp"
#include <iostream>
class A{
public:
	int i;
};

class B: public A{
public:
	double g;
};

class C{
public:
	int e;
};

// Questa class template da errore se istanziata con
// parametro template attuale che non deriva da A

template <typename T>
class Pippo
{
  static_assert(IsDerived<A,T>::value," Template argument must derive from A");
public:
  double c;
private:
  T* tp;
};

int main(){

  using namespace std;
  cout<<std::boolalpha;
  cout<< "B Derives from A?    " <<  IsDerived<A,B>::value <<endl;
  cout<< "C Derives from A?    " <<  IsDerived<A,C>::value <<endl;
  
  //OK
  Pippo<B> z;
  // Fails
  // Pippo<C> zz;
}
