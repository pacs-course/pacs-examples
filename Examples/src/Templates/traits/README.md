#A simple example of traits#

The trait, which is in fact also a policy, is used to

    1-identify the correct return type in case we operate on std::complex
    2-provide the correct way of performing the product of two number, specialising the case for std::complex where the product should be the inner product
  
  
# What do I learn here #
- A use of traits defined as template class.