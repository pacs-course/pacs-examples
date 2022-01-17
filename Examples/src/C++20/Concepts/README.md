# Concepts (since C++20) #

Concepts allow to impose contraints on the value of template parameter types. Indeed they allow to represent the semantic of types. 
For instance with
	template <class T>
	concept Has_data = requires(T v)
	{
		v.data;
	}
we can verify if a given type provides an object that contains a member called `data`.

With concepts you can obtain

- Safer code;
- More understandable compiler errors in cade of wrong template usage;
- More undestrandable code for the user.

With constraints you can check if:
- A statement can be compiled successfully
- A given type exists
- An instantiation of a template type is correct
- A function is non-throwing
- A function’s return type satisfies another concept
- A boolean expression evaluates to true

It is one of the major addition of C++20.