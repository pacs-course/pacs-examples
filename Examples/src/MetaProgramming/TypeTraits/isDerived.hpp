/*
 * isDerived.hpp
 *
 *  Created on: Nov 16, 2008
 *      Author: forma
 */

#ifndef ISDERIVED_HPP_
#define ISDERIVED_HPP_
/*!
 * This is an first example of metaprogramming. Some features of the C++
 * template instantiation and static member initialisation are used to craft
 * a class able to check whether a class (D) is publicly derived by another
 * class (B).
 */

template<typename B, typename D>
class IsDerived{
private:
	//! A type of size 1byte
	typedef char _Yes;
	//! A (unnamed) type of size 2bytes
	typedef struct{ char a[2];} _No;
	//! A function returning _Yes
	static _Yes test(B* );
	//! A function returning _No
	static _No  test(...);
	/*! A function that fails if D does not derive from B
	 The second statement in the body of the function is
	 needed to avoid warnings about undefined variables
	 */
	static void Constraints(D* p){B* pb=p; pb=p;}
public:
	/* An enum which is set at compile time to a values that depend
	 on the outcome of the test */
	enum {value=sizeof(test(static_cast<D*>(0)))==sizeof(_Yes)};
	enum {No=! value };
	IsDerived(){void(*p)(D*)=Constraints;}
};



#endif /* ISDERIVED_HPP_ */
