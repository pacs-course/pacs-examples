#A first example of template metarogramming#

We compute compile-time the Nth Fibonacci number (of course N should
be known at compile time).

We show three options, one using template class specialization,
compatible with c++98, the second, much more flexible (and simple)
that uses a recursive coxnstexpr function. The latter has also the
advantage that it works also if `N` is not given at compile time but
at run time. In the latter case, it resolves as an ordinary recursive
function. Il `N` is a constant expression instead, the result is
evaluated at compile time!
The third version is a (not constexpr) function that does not use recursion.

We use the Chrono utility to compare times. Compile with `DEBUG=no` if you want meaningful timings. 

You may note, by comparing the time for computing F(40) that
- the template class version and the constexpr recursive function when called with constexpr arguments give the fastest solution. Indeed the Fibonacci number is computed at compile time;
- The recursive function is much slower than the non-recursive solution when the argument is not a constant expression (which is obviously that case if it is
read from the terminal as in the example).


Why that recursion may produce inefficient code? The
code in `Fibonacci(const unsigned N)` is inefficient for two reasons

	1. if N is relatively large the number of recursion is huge. Indeed the recursions grow as 2^N. And a recursive call has a price!
	2. you are making useless computations. To compute f(30) you compute f(29) and f(28), then f(29) will compute f(28) and f(27)... but 
	f(28) has been already computed, and you are not taking advantage of it!. The non-recursive version avoids useless computation.
	
Concluding, unless you plan to use fibonacci only with constant
expression, so you let the compiler do the job (but if N is too large
it may give up and use a the standard call anyway), or you'd better
use a non-recursive version. Less nicer, I admit, but it can be more
efficient for large N. 

Compile the code first with the debugger, just typing `make clean; make`. Try the code. Then recompille with optimization with

    make clean
    make DEBUG=no. 

Run again and compare the timings! Only if optimization is on the
compiler is able to resolve at compile time F(40) (when 40 is given
literally) with the constexpr recirsive function and you have great savings!
This is not true for the template class version. Here the static evaluation is done in any case.

*A note*. May the non recursive version `FibonacciNR` be a constexpr function, so to take advantage when called with constant expression argument? Well, you have to try, contexpr function resolution is getting better and better in C++ so maybe it works. You just have to make the function constexpr and muve the definition in the header file. Anyway, if the resolution at compile time is not possible `constexpr` has the same effect of `inline`. 

#What do I learn here?#
* Different template techniques. In particular recurssive template classes. A bit weird, but very efficient: yet here you can only use constant expressions, only those are admitted as template value argument. So you loose the flexibility of a constexpr function that warks also if the argument is not constexpr (in which case it behaves as an inline function).
* That recursive functions are elegant, but not necessarily efficient! Most of the times a non-recursive implementation is to be preferred, particularly if you expect deep recursive calls.

 
