# Round-off error example: finite difference #

A simple test of forward error analysis for the computation of the
derivative of a function via centered finite differences.  It shows
the relevance of the truncation error as the step h gets smaller

* make run runs the test
* Change file data.pot to modify some of the parameters

With just `make` you make three main programs for different types of
floats. Each one produces a file that can be viewed with gnuplot (or
loaded in matlab). The script `plot_results.sh` launches gnuplot and
shows the results after you have launched the three versions of the
program. You may have a look at it.

With `make run` all this is done automatically.

The code computes the derivative also with the special
formula that does not takes differences, so it is not subject to
cancellation errors. Have a look at the code! The technique is illustrated in 
[this post.](https://sinews.siam.org/Details-Page/differentiation-without-a-difference)


# What do I learn here? #

- That roundoff errors are for real and, in particular, roundoff errors induced by cancellation may worsen the accuracy of an algorithm.
- How to compute the derivative of a function using a formula that does not suffer from cancellation errors.

Luckily, in many cases round-off errors are negligible 
(if you do things right). But it is important to remember that on a computer
you are working with floating points, not reals.

**Note** There are libraries that allow for arbitrary precision 
arithmetics. Clearly, computations with those libraries become much more costly.
So they are used only when having very high accuracy is crucial.
For instance, the code `triangle` by Jonathan Shewchuk uses arbitrary precision to compute some geometric predicates. 
