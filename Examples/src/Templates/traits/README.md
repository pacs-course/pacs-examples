#A simple example of traits and of automatic return type deduction#

The trait, which is in fact also a policy, is used to

    1-identify the correct return type in case we operate on std::complex
    2-provide the correct way of performing the product of two number, specialising the case for std::complex where the product should be the inner product
  
  We also show an example of automatic return type deduction where
  traits are not needed becouse we rely on the return type of the std
  function `std::abs()` which return the absolute value for a POD and
  the modulo for a `complex<T>`.
  
