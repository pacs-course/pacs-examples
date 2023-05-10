# A strange way of computing pi, openMP version

The code computes an approximation of pi by integrating 4/(1+x^2) in the interval (0,1) using a composite integration rule. It is just an example that replicates an analogous example in `MPI/Pi`, so the general description may be found in the README.md file stored in that folder. 

Here, we present 2 OpenMP versions

The first, defined in `main_pi.cpp`, is less efficient. We create a team of threads with a `omp parallel` directive
	
	#pragma omp parallel private(x), firstprivate(sum), shared(pi,h) num_threads(num_threads)

Note that `sum` is defined here as a **private** variable. `firstprivate` ensures that the local variable is initialised with the 
value of the variable with the same name already defined by the program. In this case the value is 0. This way we endure that sum is correctly initialised. the variable `pi`, also initialised by 0, is the global variable that will eventually store the desired approximation.

We then perform a for loop in parallel, within the parallel section.

	#pragma omp for schedule(static)
	for(unsigned int i = 0; i < n; ++i)
      {
        x = h * (i + 0.5);
        sum += 4.0 / (1.0 + x * x);
      }
 Remember that here `sum` is private, it means that each thread has a different instance.
 Eventually, we accumulate the local `sum`, with an atomic update
 
	#pragma omp atomic update
	pi += h * sum;
 
 `omp atomic` is here better tha a `omp critical`, since we just doing the update of a single variable.
 
 After compilation with `make`, you may call the program with
 
 ```
 ./main_pi -t num_threads -e number_of_elemts
```
if you do not specify the number of threads and/or the number of elements, the defaults are 2 and 1e9, respectively.

The second version, in `main_pi_reduction.cpp`, used a reduction clause. It is simpler an possibly more efficient. Here a snippet:

```
#pragma omp parallel for schedule(static), private(x), num_threads(num_threads), reduction(+ : sum)
  for(unsigned int i = 0; i < n; ++i)
    {
      x = h * (i + 0.5);
      sum += 4.0 / (1.0 + x * x);
    }

  // Now the main thread has the correct value of sum.
  const double pi = h * sum;
```
You may call the program with
 
 ```
 ./main_pi_reduction -t num_threads -e number_of_elemts
```


**A Note:** You may note that the result obtained with a different number of processes, for a given number of elements, may differ slightly. This should not be a surprise: the order of operation is different, so roundoff errors accumulate differently. This is a classic result of paralellisation: the result is not exacly the same as the scalar counterpart, and, moreover, it may differ depending on the number of processes. The important thing is that the differences are within the approximation error.


# What do I learn here?

- A use of the `firstprivate` clause
- The use of reduction in openMPI
- The use of GetPot to parse program parameters
- The effect of roundoff error propagation.