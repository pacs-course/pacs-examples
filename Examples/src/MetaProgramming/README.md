# A set of examples explaining metaprogramming techniques #

* *DecltypeDeclval* An example showing use of `decltype` and `declval` to interrogate the type. It also shows the use of type traits
	like `is_default_constructible` and `is_constructible`. Moreover, I use the runtime type identification techniques to interrogate the types, just
	to check the results of the type traits, which however test properties of types **at compile time**.
	
* *GCD*	Different possible implementation of a function that computes the greatest common divisor between two integers. Note also the use of 
	static_asset to verify some conditions run-time. **Note:** C++17 has added in the Standard library functions for greatest common denominator and minimum common divisor, so it is now better to use them.
	
* *ifthenelse* My implementation of `std::conditional`. Since we have the type trait `std::conditional` this class is useless, but it is a nice example of 
	template metaprogramming: you choose the type in function of the value of a bolean.
	
* *IsClonable.* An example to show how to check if a class has a particular method, in this case the method `clone()`. Now C++17 has introduced the type trait `std::void_t` which can be used instead of my handcrafted one. You can also check if `clone()` returns the right return type, but it is too involved so I decided to omit the test.

* *Metadot* Implementtion of the dot product between two std::array. We use recursion to unwrap the loop!

* *Pi*  Computing Pi using a truncated series. You can specify the number of elements of the series you wish to use: recursion is used. 
	You can also specify in a tempalte argument which type of float to store the result.
	
* *Switch*	We show first how types stored in a class can be used to determine template derived types. A mesh class selects automatically 
	the type of the boundary elements according to the type of the domain elements. We show also an approach where an integer representing
	the intrinsic dimension is mapped to the correspondint element.

* *transposeView* An example of a view. A view is a special proxy class that allows to give a different semantic to an object. In this case, we have a class 
	that provides the traspose view of a Matrix, whose actual type is passed as template parameter. So it applies to all Matrix classes that
	have the call operator (i,j) to address A_{ij}. **Things to note**: 1) the use of a trait so that some types may be specialised according to the
	specific Matrix, 2) the use of `std::is_const<Matrix>` to verify if the stored matrix is const (in this case I cannot change the elements).
	
* *Trivial*. This example shows the difference and some application of `std::is_trivial`, `std::is_trivially_constuctible` and `std::has_standard_layout`. If an object is trivially constuctible can be serialized (sufficient condition!) if it has standard layout it may be converted to pointer to the
 	first member or passed to a c/fortran function as a normal struct (in c) or a structure type in Fortran. Again, it is just a sufficient 
 	condition.
