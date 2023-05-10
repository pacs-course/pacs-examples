# A strange way of computing pi

The code computes an approximation of pi by integrating 4/(1+x^2) in the interval (0,1) using a composite integration rule. It is just an example to show the use of `MPI_Reduce`, the collective communication tool that performs a reduction.

It is also an example of the use of the tool `GetPot` to parse commmand arguments. Indeed, the code can be launched as

	mpirun -n num_procs ./main_pi [-h] [-n num_elements]
	-h print an help
	-n num_elements specify the number of integration elements (default 10e9)
	num_procs the number of MPI processes to use

The program times the operation using MPI timing tools. 
You need to use a large value of `num_elements` to see some advantage of parallelization, since the code is already very efficient.

In the code we use `MPI_Allreduce` that, differently to `MPI_Reduce`, broadcasts the ruceced variable to all processes. If you use `MPI_Reduce` only the root process will hold the reduced variable. Clearly `MPI_Allreduce` is more expensive, so use it only if necessary (it is a better choice than calling `MPI_Reduce` immediately followed by a `MPI_Bcast`).

**A Note:** You may note that the result obtained with a different number of processes, for a given number of elements, may differ slightly. This should not be a surprise: the order of operation is different, so roundoff errors accumulate differently. This is a classic result of paralellisation: the result is not exacly the same as the scalar counterpart, and, moreover, it may differ depending on the number of processes. The important thing is that the differences are within the approximation error.


# What do I learn here?

- The use of reduction in MPI
- The use of GetPot to parse program parameters
- MPI timings tool, an alternative to the native C++ ones ised in the `chrono.hpp` utility.
- The effect of roundoff error propagation.