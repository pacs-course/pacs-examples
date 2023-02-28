# Numeric limits and numerical constants #

A simple program that shows the main utilities provided by `std::numeric_limits`. This utility allows to interrogate some characteristics of basic type, like the
machine epsilon, the maximum representable number etc. It also provides some tools to assess some capabilities or characteristics of our floating point system. In particular
if it has the `Inf` and `NaN` symbols. Look at the code, it is commented heavily.

We also show a recent addition to the standard library, provided in the header `<numbers>`. This header defines, as constant expressions, several mathematical constants, in the
different floating point resolution (`double`, `long double`, `float`). In particular you have Pi, the Euler number e, and, since they are often needed, the square root of 2 and the golden ratio. 
Being set as constant expressions 
you are sure that the compiler will try to resolve expressions containing them at compile time, whenever possible.

### Quiet and non quiet NaN, what's that? ###

In IEEE compliant floating point arithmetic, `NaN` indicates the result of an invalid arithmetic operation. `NaN` are viral: any arithmetic operation involving a `NaN` producees a `NaN`.
In an IEEE compliant computer architecture, the issue of a `NaN` does not stop computation, for efficiency reasons However, normally the production of a `NaN` issues a signal that is recorded, and
you have way of checking it (we have a specific lecture on this). 

However, traditionally `NaN` is also used to indicate **missing data**. So if you store experimental data in a vector, to indicate that a datum is not available
some adopt the trick of setting it to `NaN`. However, in this case you do not want to issue any signal! You are not making an incorrect arithmetic operation. So, for this purspose you should
use a *quiet `NaN`* also called *non-signalling `NaN`*. That's why among the utilities in  `numeric_limits` you have the one that produces a quiet `NaN` and one that asks the 
system whether they are supported (not all architectures have quiet `NaN`s).

**An Important Note!** In modern C++ you do not need using the `NaN` trick to indicate missing data. **We have `std::optional` for this purpose!**.


**A hint:** If you want to print a floating point with the maximal precision allowed by its type you can do (here for a `double`)
    
    std::cout<<std::setprecision(std::numeric_limits<doube>:digits10 +1)<< mydouble
(you need to include `<iomanip>` for `setprecision`). In the folder `Utility` you have `scientific_precision.hpp` that contains a function to set an output stream to maximal precision.

**A compilation note** Compilation of the executable provided in this directory gives a warning about integer overflow. This warning in fact normally hides a nasty error! 
Remember that if you add 1 to the maximal representable (signed)
integer you normally get... the minimum representable integer! (and no runtime error). In this specific case, however, the warning is not an error since one of the
objectives of the example is to show what happens in practice when you do an integer overflow.

# What do I learn here? #
- A set of very useful tools. For instance `std::numeric_limit<double>::digit10` tells you how many significant digits you have in a double and 
`std::numeric_limits<double>::epsilon()` is the equivalent of `eps` in Matlab.

