#numeric limits#

A simple program that shows the main utilities provided by `std::numeric_limits`. This utility allows to interrogate some characteristics of basic type, like the
machine epsilon, the maximum representable number etc. It also provides some tools to assess some capabilities or characteristics of our floating point system. In particular
if it has the `Inf` and `NaN` symbols. Look at the code, it is commented heavily.

#What I learn here?#
- A tool that may be very useful. For instance `std::numeric_limit<double>::digit10` tells you how many significant digits you have in a double and 
`std::numeric_limits<double>::epsilon()` provides the equivalent of `eps` in Matlab.



