# Odd-even transposition algorithm for parallel sorting

In this example we have implemented the odd-even transposition algorithm illustrated in the book by Pacheco and Melenk. It is also an example
of the possible combination of MPI with native C++ parallel (multithreaded) algorithms, and an illustration of the use of `MPI_Sendrecv`


Let's first illustrate briefly the algorithm. Let \cpp{v} be a large vector that we want to sort according to a weak ordering relation (in this case is just `<`, so the vector's elements will be sorted from smallest to largest). We have a disposal a fasst scalar (or, as we will see multithreaded) sorting algorithm: `std::sort()`, which provides an efficient implementation of the [quicksort](https://en.wikipedia.org/wiki/Quicksort) algorithm. Quicksort is O(nlog(n)), where n is the size of the vector. We will implement a parallel algorithm that splits the vector among the available `p` MPI processes and is guaranteed to converge in `p` iterations at most.

Let's illustrate the algorithm.

1. Split `v` into `p` chunks of approx. equal size and distribute it across processes. We indicate with `v`<sub>i</sub> the local vector in process ranked `i`.
2. On each process sort `v`<sub>i</sub> using the available sorting algoritm, `std::sort` in our case;
3. For k=0,...,p-1
    * if k is even processs (0,1) (2,3) ... communicate their local vectors each other (a process may be idle)
    * if k is odd  processs (1,2) (3,4) ... communicate their local vectors each other (at least s process is left idle)
    * each couple (i,j) select the best values from the merge of the two local vectors (theirs and that of the companion). Best means that the process with lowest rank, i, keeps the lowest values, that with the highest rank the largest values.
4. Local vectors are gathered and the gathered vector is the sorted one.
     

If cp is the cost of communications (proportional to p) the final algorithm will have a computational complexity of the type (we do p iterations)

Cost=cp + ap(n/p)log(n/p)= cp + anlog(n/p), a being a positive quantity

Assuming n much larger than p, we can neglect communication cost, the parallel algorithm will have an ideal efficiency

E= anlog(n)/(panlog(n/p))=log(n)/(p log(n/p)) ~  1/p (for large n)

Therefore, it may be convenient only for small p. It is not a good algorithm since it scales very badly. It is here only to schow a simple implementation of `MPI_Sendrecv` and also a few c++ tricks.

A more interesting algorithm is parallel quicksort, whose message passing implementation is rather heavy since it requires a lot of communication. It is more suited for a shared memory environment and indeed it is reported in `Parallel/OpenMP/Sort`. 

The code can be compiled in two ways

	make all
or

	make parallel_cpp
	
in the latter case you have the native C++ parallel implementation for `std::sort()` (see the function `sort_vec_int(std::vector<double> &v)` in the file VectorUtil.hpp). Remember that if you have compiled the code in one way and you want to recompile it in a different way you have first to launch `make clean`.

To launch the code do

	mpirun -n p ./main_sendrecv [-h] [-size number_of_elements]
	-h Prints an help
	p is the desired number of processes
	number_of_elemments is the size of the vector to sort (default 1.e8).
	
Code options are parsed using `GetPot`.
The code internally does the timings using the utility `chrono.hpp` (alteratively, you may want to use the native MPI timing routines).
The vector is initialised with random values distributed uniformly in the range [-100,100), using the random number generation utilities of C++.
Eventually, the program tests whether the global vector has been effectively sorted.
Timings are taken with both the `chrono.hpp` utility and the MPI native timings.

## An important note
This algorithm is not very efficient. In particualr, on normal PC the parallel time is normally higher (even much higher) than the serial counterpart.
This types of algorithms may be beneficial in machines with very fast communication among processes, but inn general tere are better choices. 


# What do I learn here?
- How to use `MPI_Sendrecv()` in a cyclic point-to-point communication, and the use of `MPI_PROC_NUL`
- The use of `GetPot` to parse program options
- The use of `std::sort`, in its classic and (multithreded, native c++) parallel implementation.




	
