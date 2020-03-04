#Computes the 2-norm of a `std::vector<double>` or of a `std::vector<std::complex<double>>`# 

  It is an example of the use of `std::enable_if` to select a specific function member depending on the type
  of the template argument. We activate the SFINAE paradigm on the return type of the call operator.
  
  **Take it only as an example**. The function `std::norm` works also for floating point types, so you do not
  need to have two separate call operators, you might just use the second version (taking away the `enable_if` stuff of course).
  
  