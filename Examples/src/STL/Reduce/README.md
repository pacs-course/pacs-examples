# Example of parallel algoritms #

We show the `reduce` and `transform_reduce` algorithms. They can be
used to implement reduction opearations (computation of a scalar from
one or thwo vectors, with the possibility of specifying parallel
execution.

The code runs different useful algorithm both in parallel and sequential execution, timing them with the Chrono utility. You can see the difference in your machine (assuming you have more than one core!).

**Notes**

- you need to have the TBB (threading building block) layer installed (apt-get install libtbb in Ubuntu). This is true for all parallel algorithms.
- The code is compiled by default with optimization activated. Indeed in `Makefile.inc` you have `RELEASE=yes`. In my makefile `RELEASE=yes` of `DEBUG=no` mean *compile with optimization level `-O3`*

# What do I learn here? #

- The use of some powerful algorithms of the standard library;
- How to indicate the parallel policies native of the C++ language. I recall that there are other policies, non only `par` and `seq`. Try also the other ones! 