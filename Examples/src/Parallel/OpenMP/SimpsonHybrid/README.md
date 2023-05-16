# Numerical integration with a parallel Simpson composite quadrature, hybrid implementation.


This code illustrates a hybrid MPI-OpenMP implementation of the same code contained in `MPI/Simpson`. We leave the general description to the README file in that folder.

We only show the difference. The function to compute the composite rule has been changed in 

	inline double SimpsonRule_mt(std::function<double (double const &)> const & f,
                          double a, double b, unsigned int n, unsigned int num_threads)
	{
	  double integral{0.};
	  double h=(b-a)/n;
	#pragma omp parallel for num_threads(num_threads) shared(a,h,f,n) reduction(+:integral)
	  for (auto i= 0u; i< n;++i)
	    {
	      integral+=(f(a+i*h)+4.*f(a+(i+0.5)*h)+f(a+(i+1.)*h));
	    }
	  return (h/6.)*integral;
	}
where a `parallel for` directive with reduction clause is used to compute the contribution to the integral by the given MPI process.

The number of threads are given in the json file `data.pot`, and may be changed at will. Integrand function is defined in the main program, it can be done better.

 The code is then compiled with `make` and then run as usual. For instance,
 
 ```
 mpirun -n 2 ./main_simpsonHybrid
 ```
 if you want 2 processes. 
 
# What do I learn here?
 
- A simple usage  of the json reader. Much, much more in [Json for modern C++](https://github.com/nlohmann/json)
- The use of `MPI_Reduce()` to collect partial sums
- The use of `omp parallel for` directive with reduction clause.
- The C++ function wrapper
- The use of a C++ `std::tuple<>` to pack heterogeneous data. 

 
 