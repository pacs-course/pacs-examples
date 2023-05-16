# Scheduling strategies

This simple program wants to illustrate how the different scheduling strategy for a parallel for loop operate.

We recall the meaning of the different scheduling strategies, introduced with the clause `strategy(type, chunk_size)`, where `chunk_size` is optional.
In the following, `p` is the number of threads, `n` the size of the loop.

- static. Is the default. The iterations are distribuited approx. equally among threads in a round-robin fashion: the first `n/p` (or `n/p+1`) iterations are assigned to thread 0, the second to thread 1 etc. in a cyclic fashion. If you specify the chunk size, the number of iterations distributed is as uniform as possible, with a maximum represented by the chunk size.  It is the default since in most cases is a good strategy. But it may be inefficient if every iteration may take a different amount of time. In that case `dynamic`or `guided` may be a better choice.

- dynamic. Unless specified, chunk size is here equal to 1. A number of iterations equal to chunk size is assigned to an available thread. As soon as a thread completes its job is put back in the set of available threads. You may see that in this case you may have a better distribution of the workload whenever the cost of each iteration is not uniform. The problem here is the choice of the chunk size. If it is small (the default is 1), we have in principle the best situation if the cose if each iteration is highly variable. But, on the other hand, it may be advanteageous to let a thread do a larger amount of work, by assigning a larger number of iteration. The `guided` strategy tries to address the problem

- guided. The guided strategy is similar to dynamic. The chunk size is determines dynamically by the system, by decreasing it while iteration
progresses. The idea is that it is better to have small chunks at the end. If you specify `chunk_size`, you are in fact specifying the maximal size of the chunk.

- runtime. It is not a real strategy. It just indicates that the strategy is specified via the environmental variable **OMP_SCHEDULE**. For instance if we set (bash syntax)

	export OMP_SCHEDULE-"dynamic"
the clause `schedule(runtime)` will convert into `schedule(dynamic)`. We can also specify the chink size

	export OMP_SCHEDULE="guided,4"

In this code, we just run a `parallel for` loop with different strategies and chink sizes, and we print which thread has been assigned to each iteration. It is also a use on `std::map` (in fact we could have used just a vector...).



*Note 1* Note that if you run the program different times, while the distribution among threads in the static strategy doesn't change, it does for the
other two.

*Note 2* The `strategy` clause may ave an additional modifier, which can be `monotonic` or `nonmonotonic`, for instance,

	strategy(nonmonotonic:dynamic,4)
This is a rather advanced detail that we have omitted here. 


## The `omp_set_schedule` function. ## 
It is an alternative to setting the environmental variable `OMP_SCHEDULE` to define the actual strategy to be used when `strategy(runtime)` is adopted: if you have in your code

	omp_set_schedule(omp_sched_dynamic, 4)
you are setting a dynamic scheduling with chunk size 4 whenever a `strategy(runtime)` is used. 

 

# What do I get here?
- Scheduling strategies shown with a simple example;
- A use of `std::map`. You can try to use an `std::multimap` to invert the relation: give the iterations assigned to each task.
