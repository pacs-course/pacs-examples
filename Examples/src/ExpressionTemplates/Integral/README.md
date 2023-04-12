# Example of use of expression templates to create efficient code for numerical integration. #

The drawback: the integrand must be known at compile time, since the
compiler has to resolve the expression.

For compiling the example use make ``DEBUG=no`` (to activate
optimization). If you like the code and you want to install it
somewhere to use it for other purposes , since it is a template only
library you just have to copy all files in the ``include/`` directory in
the directory where you normally keep your header files.

    
