#Usage of the random distribution utilities available in the Standard library#


The file `main_random.cpp` produces a set of samples generated with
different distributions. It then generates a set of data according to that distribution and computes some statistics of the sample.

It also plots an istogram of each sample using the gnuplot-iostream facility.

**Note** You must have gnuplot and the boost libraries installed in your system
    to use gnuplot-iostream. If not you can add `CPPFLAGS+=-DNO_GNUPLOT` in `Makefile.inc` and the istogram data will be printed on the screen instead.

It also some files with extension `.dat` with the values produced by
each distribution. The number of samples may be changed by setting the
constexpr variable `N` in the source file.

The data file produced by main_random may be seen with `gnuplot`. You only
need to skip the first line, which contains the number of samples.
You can use this trick, at the glupolt prompt

    gnuplot> set key autotitle columnhead
    gnuplot> plot "filename" w p title "samples"

Or

    gnuplot>plot "filename"  every ::1


