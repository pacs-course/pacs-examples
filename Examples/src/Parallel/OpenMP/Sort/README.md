# Comparison of different parallel sorting algorithms

In this example we compare different sorting routines:

1. The standard serial `std::sort`, which implements qicksort;
2. The multithresed version of `std::sort` tat implements a parallel version of qucksort obtained with the native multithreading facilities of C++;
3. The parallell (openMP) version of the  odd-even transposition sort, whose description may by found in the boof *Introduction to Parallel Programming* by P.S. Pacheco and M. Malensek. **However, being this algorithm rather inefficient, this version is currently commented-out and disabled**.
4. A parallel QuickSort algoritm based on OpenMP, which is a modified version of the code presented in [this site](https://github.com/Michael-Beukman/OpenMPQuicksort)

The QuickSort algorithm is one of the major algorithm for sorting, with Nlog(N) complexity. It is based on a recursive subdivision and rearrangement of the original vector. A full description of the algorithm (and all its variants) may be found on [Wikipedia](https://en.wikipedia.org/wiki/Quicksort).


The code can be compiled in two ways

	make all
or

	make parallel_cpp
	
in the latter case you activate also the native C++ parallel implementation for `std::sort()`. Remember that if you have compiled the code in one way and you want to recompile it in a different way you have first to launch `make clean`.

To launch the code do

	main_sort [-t p] [-h] [-size number_of_elements]
	-h Prints an help
	p is the desired number threads
	number_of_elemments is the size of the vector to sort (default 1.e8).
	
	
Some peculiarities of the code:

- The use of `omp_set_num_threads(num_threads);` to set the default number of threads. This way we do not need to specify the claus `num_threads` in the `omp parallel` directive.
- The use of tasks to indicate the portions that can  be carried out in parallel.

```
template <class T>
void par_q_sort_tasks(int p, int r, std::vector<T>& data){
#pragma omp parallel
        {
            #pragma omp single
            q_sort_tasks<T>(p, r, data, TASK_LIMIT - 1);
            #pragma omp taskwait
        }
}

template <class T>
void q_sort_tasks(int p, int r, std::vector<T> & data, int low_limit) {
  if (p < r) {
...
           int q = partition<T>(p, r, data);
            // create two tasks
#pragma omp task shared(data)
            q_sort_tasks<T>(p, q - 1, data, low_limit);
#pragma omp task shared(data)
            q_sort_tasks<T>(q + 1, r, data, low_limit);
        }
}
```

We recall that `omp task` should normally be launched by a single thread of a team of threads (that's why we have `omp single`) and defines a task that can be carried out concurrently with other tasks in the same section. 

`#pragma omp taskwait` imposes a synconization of the tasks. Note that the algorithm is recursive, so, even if `q_sork_task` defines just two tasks, the recursion will generate other two couples and so on. The taks that can be done cuncurrently will be assigned to the available threads.

- The use of a limit (here set to 1000, via the C preprocessore macro `TASK_LIMIT`) of the number of elements for the parallel version. All partitions with less elements will be processed with the sequential version of the algorithms in `seq_qsort()`.

We can see that for rellatively small number of elements (<10000) the native serial version, `std::sort` is by far more efficient. Increasing the size the parallel versions get more efficient, the C++ native parallel `std::sort`, being highly optimized, becomes the most efficient for large sizes (>1M). 
For intermediate sizes, the native C++ parallel sort and the "hand made" parallel quicksort behave similarly.
Of course, these result refer to my PC, which has just 4 core with a max of two independent threads each. Performance must always be assessed on a specific architecture. 


*A Note*
The parallel odd-even transposition algorithm presented in the book of P. Pacheco is not very efficient, at least on my computer.
The main reason is the high number of cache miss. Differntly than the quicksort algorithm, concurrent threads work on adjacent data
and the risk of cache miss is high. We do not go in more details here, in the book by P Pacheco and M. Malensk you have a thorough discussion about cache and how cache misses may slow down shared mamory parallel code.


# What do I learn here?execution
- A use `omp task`  and of `omp_set_num_threads()`;
- The use of `GetPot` to parse program options;
- The use of `std::sort`, in its classic and (multithreded, native c++) parallel implementation;
- A nice implementation of parallel quicksort.

# QuickSort algorithm pseudocode (serial version)

For completeness we report a pseudocode of the quicksort implemented in the example.

```
// Sorts a (portion of an) array, divides it into partitions, then sorts those
algorithm quicksort(A, lo, hi) is 
  // Ensure indices are in correct order
  if lo >= hi || lo < 0 then 
    return
    
  // Partition array and get the pivot index
  p := partition(A, lo, hi) 
      
  // Sort the two partitions
  quicksort(A, lo, p - 1) // Left side of pivot
  quicksort(A, p + 1, hi) // Right side of pivot

// Divides array into two partitions
algorithm partition(A, lo, hi) is 
  pivot := A[hi] // Choose the last element as the pivot

  // Temporary pivot index
  i := lo - 1

  for j := lo to hi - 1 do 
    // If the current element is less than or equal to the pivot
    if A[j] <= pivot then 
      // Move the temporary pivot index forward
      i := i + 1
      // Swap the current element with the element at the temporary pivot index
      swap A[i] with A[j]

  // Move the pivot element to the correct pivot position (between the smaller and larger elements)
  i := i + 1
  swap A[i] with A[hi]
  return i // the pivot index
```
	
