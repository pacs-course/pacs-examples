#A first example of template metarogramming#

We compute compile-time the Nth Fibonacci number (of course N should
be known at compile time).

We show two options, one using template class specialization,
compatible with c++98, the second, much more flexible (and simple)
that uses a recursive coxnstexpr function. The latter has also the
advantage that it works also if `N` is not given at compile time but
at run time. In the latter case, it resolves as an ordinary recursive
function. Il `N` is a constant expression instead, the result is
evaluated at compile time!

However, remember that recursion may produce inefficient code. The
code in fibonacci(const unsigned N) is inefficient for two reasons

	1. if N is relatively large the number of recursion is huge. Indeed the recursions grow as 2^N.
	2. you are making useless computations. To compute f(30) you compute f(29) and f(28), then f(29) will compute f(28) and f(27)... but 
	f(28) has been already computed, and you are not taking advantage of it!.
	
Concluding, unless you plan to use fibonacci only with constant
expression, so you let the compiler do the job (but if N is too large
it may give up and use a the standard call anyway), or you'd better
use a non-recursive version. Less nicer, I admit, but it can be more
efficient for large N. A possible and working implementation is in
`fibonacciNR`.

Compile the code first without the debugger, just typing `make clean; make`. Try the code. Then recompille with optimization with

    make clean
    make DEBUG=no. 

Run again and compare the timings! Only if optimization is on the
compiler is able to resolve at compile time F(40) (when 40 is given
literally) and you have great savings with the constexpr recursive
version. If you give the input as a variable the recursive version is
instead much slower than the non-recursive one.


 
