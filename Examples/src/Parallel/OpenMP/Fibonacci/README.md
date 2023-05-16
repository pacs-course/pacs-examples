# Code for generating Fibonacci numbers #

Here we have implemented a recursive algorithm for th egeneration of Fibnacci sequences, and a parallel version using OpenMP *tasks*.

The use of tasks is here necesssary because of the nature of the algorithm, which is not formulated as a loop or as a fixed, predetermined number of parallel parts.

We indeed use a recursion based on the formula

	F(k) = F(k-1)+ F(k-2), k=n,n-1,..1; F(1)=1, F(0)=0;
	
A (very moderate) level of parallelization is achived by noting that the computation of `F(k-1)` and `F(k-2)` can be carried out concurrently. Therefore, we can assign the computation to two different tasks:

```
  long unsigned int fib(long unsigned int n)
  {
    long unsigned int i{0ul};
    long unsigned int j{0ul};
    if(n<=1ul) // break recursion
      {
        fibs[n]=n;
        return n;
      }

#pragma omp task shared(i) if (n>par_limit)
    i = fib(n-1ul); // compute F(n-1)
#pragma omp task shared(j) if (n>par_limit)
    j = fib(n-2ul); // compute F(n-2)
#pragma omp taskwait // syncronize tasks
    fibs[n]= i+j; // store value F(n)
    return fibs[n]; // return last value
  }
```
The creation of the teams of threads is done by the public method `compute()`:

```
  auto const &
  compute(long unsigned int n)
  {
    fibs.resize(n+1ul,0ul);
#pragma omp parallel num_threads(num_threads)
#pragma omp single
    fib(n);
return fibs;
  }
```

Note the use of some namespace inline variables. A variable is declared inline when is defined in a header file and you want to avoid problems with the one definition rule (since C++17).

The parallel code is in fact **higly inefficient**, let's see why. Each task has to do very little work (an addition of two integers!), and (if we ignore the `if (n>par_limit)` clause) the computation of `compute(n)` generates `2^(n-1)` tasks!.
The overhead of taks creation and destruction may exceed greatly the advantage of parallel computations. Moreover, differently than the example in forlder `Sort` we have a `taskwait` clause that obliges sincronysation of each couple of tasks.

This example shows also a serial nonrecursive implementation. Just to show that recursive algorithm are elegant but may be inefficient!. **The non recursive version is (at least on my computer) the fastest, and by far!**. For two reasons: 1) you do not have the overhead of repeated function calls, 2) in this case, the non recursive version is much more *cache friendly* than the non recursive one, since the iteration access consecutive elements.


# What do I learn here #
- That recursive algorithm may be difficult, if not impossible, to parallelize efficiently, in particular in  machines with 
a small number of cores. Better results are in the `Sort` folder, where we show a parallel implementation of another recursive algorithm.
- The serial non recursive version of the algorithm cannot be made parallel, but (at least on ordinary PCs) the fastest, and by far! 
- A possible use of inline variables in c++











 