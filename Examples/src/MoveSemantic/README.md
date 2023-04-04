# The advantage of move semantic #

An example of matrix class using templates as policy for storage order.
Furthermore, we implement move sematic: a move constructor and
a move assigment operator have been defined.

`make` produces two executable, one where move semantic is activated, in
    the other is deactivated. It then runs `valgrind` with the `massif` toolbox
so you can compare the difference in memory usage between the versions.

**Note:** You must have valgrind installed

**Note:** I use `ms_print` to produce a readable output from the file `move.out`
produced by `valgrind`. You may use other, fancier, tools.

# What do I get from this example #
- That when handling large objects whose memory is dynamically allocated move semantic may help in reducing memory footpring quite a lot!
- That using standard container to store dynamic data is a good practice since they have move semantic already in place, and you have not too worry too much about it;
- The use of one of the many [valgrind](https://valgrind.org/) tools for profiling your code.
