# Tuples #



- `printtuple.hpp` Rather complicated stuff. Have a look only if
  interested. It provides a streming operator for tuples using
  template recursion. Used in the other examples. The use is simple,
  the implementation is not!
- `HashTuple.hpp` An utility that implements an hash function for tuples. It assumes that all elements
  of the tuple have an `std::hash` implementation provided by the standard library. So *it cannot be used* for 
  nested tuples, i.e. tuples of tuples. The implementation is a porting to c++17 of the similar utility present
  in the boost library `boost::functional`.
- `test_tie.cpp` A particular use of tie: create a comparison operator
  that does lexicografic comparision using elements of different
  type. It relies on the fact that tuples have a comparison operator,
  based on lexicographic comparison.
- `test_tuple.cpp` A program that shows the main functionalities of a tuple.
- `test_tuple_cat.cpp` A program showing how to concatenete tuples.

# What do I get from this stuff?#

The main aspects of tuples, an important and useful container of the Standard Library.

