# The dreaded data race

A major problem, and typical error, in shared memory parallelism is related to data race
We recall the distinction between Race Condition and Data Race

A *Race Condition* occurs when the timing or order of events affects the correctness of a piece of code and the parallel execution may make the order indetermined. We have seen in the `Hello-World` example how the order of printing of parallel threads is not determined. If this is a cause of incorrect program behaviour, we have a Race Condition.

A *Data Race* occurs when one thread accesses a shared resource while another thread is writing to it. The final result is indetermined (and typically wrong).

In this case we show a typical example of condition that creates a situation that can be considered both a Race Condition and a Data Race. We have the loop

	for(size_t i = 0; i < n - 1; ++i)
    	{
      	a[i] = a[i + 1] + b[i];
	}

and (incorrectly), we parallelize it using

	#pragma omp parallel for
	for(size_t i = 0; i < n - 1; ++i)
    	{
      	a[i] = a[i + 1] + b[i];
	}
The `omp parallel` directive spawns a certain number of threads and the default schedule strategy for the `parallel for` is `static`: the iterations are split among threads in chunks of approx same size in a round-robin fashion: the first thread gets the first chunk and so on. The size of the chunk is, in this case, computed by the system. Since the threads runs independently there is no guarantee that when a thread accesses `a[i+1]` with,for instance, `i=3`,  `a[i]` with `i=4` has not already been updated by another thread. We have a race condition, since the order of execution of the loop iterations affects the final result. We have potentially also a data race, since the writing of `a[i]` with `i=4` may happen contextually to the reading of `a[i+1]`, with `i=3`. And the result is again undetermined.

Unfortunately, there is no easy way to parallelize that loop  (you may give it a try). Compile and run the code different times, so you will appreciate the undeterministic nature of the error.

*Note* You may try to change scheduling strategy, the error persists.

# What do I learn here?

A simple typical loop which causes a race condition.





