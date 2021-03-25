# The remove utility #

Unfortunately, sometimes in C++ names may be misleading. And a typical example is `std::remove`, available if you include `<algorithm>`.

**`std::remove` and `std::remove_if` don't remove anything!** 

They operate on a range (i.e. any container, here we see the example for a
`std::vector`) by moving at the end of the container the "removed"
elements and return an iterator to the position where the "removed"
portion begins.

So removing 3 in `[1,2,3,4,5]` generates `[1,2,4,5,*]`, where * is a undefined value, and return an iterator to element *. 
Why this peculiar behavior? I do not know.

To make `remove` remove you need to use it in combination with the method
`erase()` of the container, as shown in this example.

Just to make things more confusing, on a `std::vector` the method
`erase()` does not change the capacity of the vector, only possibly
its size. Here however, the reason is clear. Changing the capacity
implies memory reallocation, since in a vector data is contiguous in
memory. And memory reallocation is expensive and dangerous (it
invalidates possible pointers or iterators to the data). So, the user
has to activate it explicitly using `shrink_to_fit()`. *This is not necessary if memory is not an issue:* 
the vector has the correct size and this is enough.

Note that in other containers, where the elements are not contiguous
in memory, `erase` actually frees the memory allocated by the erased
elements.

This example shows what happens in the different cases: using only remove, remove + erase and remove+erase+shrink_to_fit.

# What do I learn here#
 -The use of `remove` and its strange semantic.
 
# A note#
C++20 has introduced a new version of `erase` that does also the job of remove+erase (finally!).
