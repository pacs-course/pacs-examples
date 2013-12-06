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
class Pippo: private IsDerived<A,T>
{
public:
	double c;
private:
	T* tp;
};

int main(){

	using namespace std;

	cout<< "B Derives from A?    " <<  IsDerived<A,B>::value <<endl;
	cout<< "C Derives from A?    " <<  IsDerived<A,C>::value <<endl;
	//OK
	IsDerived<A,B> testing;
	// Fails!
	// IsDerived<A,C>();

	//OK
	Pippo<B> z;
	// Fails
	// Pippo<C> zz;
}
