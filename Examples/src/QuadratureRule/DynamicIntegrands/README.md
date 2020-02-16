#Example of dynamic loading of integrands#

**Note:** YOU NEED TO INSTALL THE MAIN QUAQDRATURE RULE LIBRARIES FROM
THE baseVersion directory. Otherwise the example does not work.

`make dynamic` compiles the library `libintegrands.so` with the integrands

`make exec` to compile executable

`DEBUG=no` for optimized version

`./main_integration -h` gives instruction on the parameters

We usr GetPot to read a file with the parameter and the indication of the integrand.
