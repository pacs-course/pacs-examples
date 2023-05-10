# Two simple "hello world!" programs

The two executables in this directory show the basic features of an OpenMP program.

In `main_hello_world_simple`, after the program prints

	  std::cout << "Hello World!" << std::endl;
	
we launch the openMP directive

	#pragma omp parallel num_tread(8)
	
 that introduces a parallel region where 8 threads print 

    std::cout << "Hello World (this time in parallel!)" << std::endl;

The lines of code outside the openMP region introduced by `#pragma omp parallel` are executed in serial mode. The `#pragma omp parallel` directive forks additional threads (7 in this case) and the 8 threads execute the code in the structured block (here formed by just one line) following the directive.

At the exit of the parallel region, the threads join into 1 and the code continue in serial mode.

Within the parallel region, the order the threads execute the commands is undetermined, that's why different runs of the code may produce garbled output.

The second version, `main_hello_world_advanced` is just a bit more sophisticated. First of all, the number of threads are asked by the program, so the user may decide how many threads to activate.

Moreover, the `omp parallel` construct is more elaborated,

	#pragma omp parallel private(thread_id) num_threads(num_threads) \
          shared(n_threads, num_threads)
          {
          // structured block
          }
Not only we specify with the `num_threads` clause how many threads we want, but we also specify that the variable  `thread_id` is private, i.e. a different instance is created for each thread, while 	`n_threads` and
`num_threads` are shared, i.e. their value is shared by all threads.

An implicit rule for private/shared variables is present, and it says that variables defined outside the openMP region are shared, those defined in the OpenMP region are private. However, usual hiding rules apply (a variable defined in a structured block with the same name of a external variable "hides" the external variable), and, to avoid errors, it's normally better to be explicit, and declare which variable is meant to be private and which ones are instead shared.

Another new feature introduced in the "advanced version" is the presence of a `#pragma omp critical` 
and a `#pragma omp barrier` directive inside the `omp parallel` construct:

	#pragma omp critical
    		std::cout << "Hello World from thread " << thread_id << "!" << std::endl;
	#pragma omp barrier

The code in the structured block following the `critical` directive (a single line in our case) will be executed by a thread at a time.

`omp barrier` instead, puts a thread in wait state until all threads have reached the point with the directive, so it is used to synchronize the threads.

Finally, `#pragma omp single` is used to ensure that just a thread executes a block of code lines:

	#pragma omp single
    {
      n_threads = omp_get_num_threads();
      std::cout << "Number of threads requested = " << num_threads << std::endl;
      std::cout << "Number of threads obtained  = " << n_threads << std::endl;
      std::cout << " -- message printed by thread n."<<omp_get_thread_num()<<" --\n";
    }

If we had written `master` instead of `single`, the lines of code would have been written by thread n. 0, the master (or main) thread. with the `omp single` directive, we do not know which thread will indeed print the message.

The code also introduces the `omp_get_thread_num()` and `omp_get_num_threads()` openMP functions.

Compared to the "simple" version, we may note

1. The output message "Hello world from thread..." is never garbled, thanks to the critical section. Yet, the order of the threads is still undetermined, and different runs may produce a different order.
2. The final message may be printed by different threads in different execution of the program.

# What do I learn here?
- Some basic features of OpenMP.




 



	