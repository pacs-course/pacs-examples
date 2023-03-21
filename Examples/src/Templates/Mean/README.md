# Overloading of function templates and a use of variadic templates #

In this Example I show two techniques

1. Overloading of function templates. Here the function template `mean` is overloaded so to operate on different types and give a resonable result. For instance, the version that takes vectors of arrays as argument is obviously different than the one taking scala arguments. Moreover I want the possibility of summing the values referencing by two pointer by simply passing the pointers (*note* this overload is for illustration of the technique, Don't do it, it can be confusing). Note that some versions have been declares `constexpr` to take advantage of possible constexpr arguments.
2. We also show the power of variadic templates. Two small utilityes `Mean` and `Sum`, have been designed to compute the mean and the sum of a variable number of (scalar) arguments.   

## A note ##
If you want to see the effect of constexpr compile the code with and without optimization and see what are the symbols contained in the executable:

```
    #without optimization 
	make -B 
	nm --demangle main | grep mean
	#with optimization
	make -B DEBUG=no
	nm --demangle main | grep mean
```
You may note that some computations have been resolved compile time (symbol not present). This is because in the example I use constant expressions as arguments for many of the calls to `mean`,
so the effect of the `constexpr` specifier is activated when optimization is on.

# What do I learn here? #
- Another use of function tempalte overloading
- A use of variadic tempaltes