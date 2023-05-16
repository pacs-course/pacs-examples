# The parallel tools of the standard library #
The standard library now support different parallel algorithms. All parallel algorithms are characterised by the specification of a particular argument, which is the first argument of the algorithm, which specifies the type of parallel execution that is desireed.
It is called the *parallel execution policy*. All algorithms, as usual for std algorithms, operate on ranges.

At the time being (but in the future more can be added) we have the following possible paralle execution policy object, that may be passed as argument: 

- `std::execution::seq`  No parallelization. The algorithm will be executed in the scalar mode. For several algorithms, this is the default if the execution policy is not specified at all.
- `std::execution::par`  The classic multi-threaded parallelization. The obeicts in the range are subdivaded into groups and each thread  performs the algorithm operation on a group. 
- `std::execution::unseq` (since c++20) A SIMD approach. The same operation is performed on several data at the same time. Meant for vector architectures or GPUs.
- `set::execution::par_unseq` indicates that a parallel algorithm's execution may be parallelized, vectorized, or migrated across threads. For mixed type architectures.

**A note:** The effective use of one or the other policies depends on the hardware and the support of the operative system. For instance, in INTEL type architecture, with the use of g++ or clang++ compilers you have to link with the threading building block library (`libtbb`), if you want that your code compile if you use a STL parallel algorithm.

##The name of the algorithms##
Sometime the name of the parallel algorithm is the same of the name of its non-parallel version. For instance, `std::sort`. When this is the case it means that
```
std::sort(v.begin(),v.end()); // non parallel version
```
and
```
std::sort(std::execution::par,v.begin(),v.(end)) // parallel version with parallel policy
```
produce exactly the same result.

The is not the case for other algorithms. For instance `std::inner_product`, a non-parallel algorithm of the STL, has a parallel equivalent `std::reduce`. Why? Because the logic of the `std::reduce` code had to be changed to make it possible to have parallelism, and it is not guaranteed that `reduce` and `inner_product` give  exactly the same result on the same range of data, because of different accumulation of round-off error (not even if you choose the `seq` policy!). To avoid problems of inconsistencies, the Standard Committee preferred to change name.

In this main program you have just some examples of use of just a few parallel algorithms of the stl, with some timings.

# What do I learn here? #
- How parallel algorithms of the standard library are called.
- How to get a parallel code cheaply using modern C++.