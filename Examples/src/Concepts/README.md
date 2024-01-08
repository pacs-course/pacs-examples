# Concepts#

Concepts allow to impose contraints on the value of template parameter types. 


Indeed they allow to represent the semantic of types. 
For instance with
	template <class T>
	concept Has_data = requires(T v)
	{
		v.data;
	}
we can verify if a given type provides an object that contains a member called `data`.

At this point we can contrain a tempalte function of class, for instance
	
	```c++
	template <Has_data T>
	void print_data(T v)
	{
		std::cout << v.data << std::endl;
	}
	
	```
	or using a constrained auto
	
	```c++
	void print (Has_data auto v)
	{
		std::cout << v.data << std::endl;
	}
	```


With concepts you can obtain

- Safer code;
- More understandable compiler errors in cade of wrong template usage;
- More understandable code for the user.

It is one of the major addition of C++20.

Another important characteristics is that overloading and template specialization can be constrained by concepts.
An example:
```c++
auto fun(auto v) // primary template
auto fun(std::integral auto v) // specialization for integral types

```


In this example I also show two attributes:

- `[[nodiscard]]`, which indicates that the return type of a function should not be discarded. The compiler will generate a warning if you discard the returned value;
- `[[maybe_unused]]`, which indicates that a variable may be unused, so no warning is generated in this case.

# What do I learn here ? #
- The use of a very recent addition to the C++ language
- The use of two special C++ attributes
