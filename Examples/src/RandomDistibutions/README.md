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

The code uses the header `StatisticsComputations.hpp`, which is in the `Utility` folder. You have to install it (following the instructions in that folder).


#What do I learn here?#
- The different random number generators and engines of the standard library
- An utility to extract statistical information from a sample (univariate)
- A use of `gnuplot-iostream`

## Important ##

`gnoplot-iostream` uses the boost libraries. So if you want the
example to be working you must have the boost library (development
version) installed. Not all of them infact, in Makefile.inc you see the one that are required: `boos-iostream`, `boost-system` and `boost-filesystem`.

If you are using the modules you have all of them with

```
module load boost
```

If you do not use the modules, modify `Makefile.inc` to suit your need!.
