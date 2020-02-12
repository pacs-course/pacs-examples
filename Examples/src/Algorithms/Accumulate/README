#An example of accumulation of roundoff errors in a sum#

We create a vector with numbers of largely differen magnitute and
different sign. The vector is constucted so that the exact solution of
the sum of the element is known.

We sshuffle randomly the elements of the vector and we sum them. We
then repeat the operation by partitioning the vector into its
non-positive and positive entries, we order each partition so that the
elements with smallest absolute value are first. We sum the two
partitions separately and then we add the result.

You may note that, while the standard summation gives very large
errors due to cancelation (loss of significant digits when you sum or
subtract elements largely different in size), the procedure with
ordering gives very small, or zero error.

You can compile with

  * make float (uses float type)
  * make double (uses double type)
  * make extended (uses long double type)

so you can compare the different results. Note that the random shuffle
is really random, so the solution with the standard sum may differ
between runs.

It is also a good example of the use of some std algorithms, CPP
variables, and of ``numeric_limits<>``.

Note that in all cases you don't get the exact result, since the exact
sum is of too many order of magnitude less than the some of the
elements in the vector. However, the ordered summetion gives a much
smaller error. If you decrease the number of elements in the vector,
eventually the ordered summation will recover the exact value, but not
the standard sum.
