#Round-off error example: finite difference#

A simple test of forward error analysis for the computation of the
derivative of a function via centered finite differences.  It shows
the relevance of the truncation error as the step h gets smaller

    *make run runs the test
    *Change file data.pot may to modify some of the parameters

With just `make` you make three versions for different type of
floats. Each one produce a file that can be viewed with gnuplot (or
loaded in matlab). The script ``plot_results.sh`` launches gnuplot and
shows the results after you have launched the three version of the
program. You may have a look at it.

With ``make run`` all is done automatically.

**NEW VERSION**: it computes the derivative also with the special
formula that does not takes differences, so it is not subject to
cancellation errors. Have a look at the code!
