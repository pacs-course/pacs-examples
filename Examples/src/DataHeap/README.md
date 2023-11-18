# A heap #

A heap is a data structure that allows you to store a set of elements and keep track of the smallest element in the set (according to a given ordering relation). It is useful for implementing priority queues.

The standard library provides a `std::heap`  that implements a heap class and a `make_heap` function that takes a vector and turns it into a heap. It also provides `push_heap` and `pop_heap` methods that allow you to add and remove elements from the heap.

However that implementation operates directly on the data vector.  In this implementation the heap structure is on top an existing data vector, whose elements are not moved by the heap operations. In other words, this heap is implemented through an indirection table. It may be useful when you whant to have an heap view of some data in a vector but at the same time you want to keep the position of the elements in the vector unchanged.

In this implementation the data vector is stored as private member and made avaliable through the method `data()`.

## Main heapView operations ##
The heap operations are:
- top(): returns the smallest element in the heap
- pop(): removes the smallest element from the heap, returning a pair contianing the value. Note that the data vector is unchenged!
- add(value): adds an element to the heap. The index of the added element is returned. The data vector inclreses its size by une: the added element is added to the back of the data vector.
- update(index,value): updtes the value of the element in the data vector at the given index. The index is returned.
- remove(index): removes the element at the given index from the heap. The index is returned. The data vector is unchanged!
- operator[](index): returns the value of the element at the given index in the data vector (not is the heap).

# What do I learn here? #
- How to implement a heap data structure
- How to use a heap 
- An important algorithms

# Note: heap and ordered sets #
You can keep track of the smallest elements in a set by using an ordered set. However, the heap is more efficient for this purpose. The ordered set keeps all elements ordered by exploiting a binary data structure. In a heap we use a linear structure and we keep track only of the smallest element. The heap is more efficient when you need to keep track of the smallest element only. The ordered set is more efficient when you need to keep track of all elements in order.

