# A few examples on floating points (and their degeneracy..) #

Here we have a few examples that show dome difficulties that may be encountered due to the fact that floating points are only **approximations** of real numbers.


- `FPComparison` Comparing two floating points for equality may fail becouse of roundoff error;
- `FPfailure` An example that show a degenerate situation where because of floating point roundoff error you do not get what you should get;
- `FinDiff` An example which shows how cancelation errors may ruin the theoretical convergence of finite difference approximation of derivatives if you take a step too small;
- `Quadratic root` How to compute the root of a quadratic reducing roundoff errors.

We have also a folder with utilities to help handling floating point exceptions:

- `FPException`

We recall that an IEEE standard compliant computer will not stop computations if we have a floating point exception (for instance if we divide a floating point with zero).
Instead, it generates special symbols, like `Inf` or `NaN`, that the program carries along, and the computations continue. However, the computer keeps track of the fact that a particular FPE has occurred and it is possible to enquire if a specific "floting point exception flag" has been raised. In this example we show not only how to interrogate
floating point exception but also how to operate to have a program that stops if a floting point exception happens (floting point exception trapping). However, remember that trapping FPEs comes with a cost, so ativate it only if you need to debug a program that is not working because of a floating point exception.  

# What do I get from these examples? #
- That roundoff errors may matter, particularly in algorithms where you make differences of floaing points with very similar value;
- How to enquire/trap floating point exceptions, like overflow or invalid floating point operation;
- How calculating quantities using less standard formulae can give a more roundoff stable algorithm;
- How some degenerate (and fortunately unlikely) situations may drive your algorithm to a wrong solution.
