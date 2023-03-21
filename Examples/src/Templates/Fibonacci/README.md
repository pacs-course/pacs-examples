#COmpting Fibonacci numbers (possibly) at compile time#

We compute compile-time the Nth Fibonacci number (of course N should
be known at compile time).

We show three options, one using template class specialization,
compatible with c++98, the second, much more flexible (and simpler)
that uses a recursive coxnstexpr function. The latter has also the
advantage that it works also if `N` is not known at compile time but
at run time. In the latter case, it resolves as an ordinary recursive
function. Il `N` is a constant expression instead, the result is
evaluated at compile time!
The third version is again a constexpr function that does not use recursion.

We use the Chrono utility to compare times. Compile with `DEBUG=no` if you want times obtained with the optimised version. 
Indeed, this examples also shiows the effects of compiler optimizations, so you may to compile with and without optimization activated to verify the difference.  

Using the optimized version (`make -B DEBUG=no` if you want to be sure to compile it) you may note, by comparing the times
- the template class version and the constexpr recursive functions when called with constexpr arguments give similar times. Indeed the Fibonacci number is computed at compile time;
- The recursive function is much slower than the non-recursive solution when the argument is not a constant expression (which is obviously that case if it is
read from the terminal as in the example).


Why that recursion may produce inefficient code? The
code in `Fibonacci(const unsigned N)` is inefficient for two reasons

1. if N is relatively large the number of recursion is huge. Indeed the recursions grow as 2^N. And a recursive call has a price!
2. you are making useless computations. To compute f(30) you compute f(29) and f(28), then f(29) will compute f(28) and f(27)... but 
	f(28) has been already computed, and you are not taking advantage of it!. The non-recursive version avoids useless computation.
	
Concluding, unless you plan to compute Fibonacci numbers only with constant
expression, so you let the compiler do the job (but if N is too large
it may give up and use a the standard call anyway), or you'd better
use a non-recursive version. Less nicer, I admit, but it can be more
efficient for large N. 

Compile now the code with no optimization, just typing `make clean; make` or `make -B` (compilation without optimization is the default.
You may note (at least this is what happens in my PC):

- The template specialization version (which unfortunately can be used only with a constexpr argument) still runs in small time.
- Also the non recursive version is still fast, even when computing f(40).
- The recursive version is terrible: the cost grows exponentially.

Why is that? Without optimization the compiler (at elast the one in my PC) has resolved the recursive functions as ordinary function,
not at run time. 


# What do I learn here? #
* A template techniques used to build  recursive relation computed at compile time. In particular recursive template classes. A bit weird, but very efficient. But here you can only use constant expressions since only those are admitted as template value argument. 
* With a `constexpr` function you can implement both a recursive and non recursive algorithm that wark also if the argument is not a constant expression (in which case it behaves as an inline function), but is resolved at compile time (at least the compiler tries to) when the argument is a constant expression.
* That recursive functions are elegant, but not necessarily efficient! Most of the times a non-recursive implementation is to be preferred, particularly if you expect deep recursive calls.
* Optimization is important to make constexpt function efficient.
 
