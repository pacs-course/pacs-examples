# A heap #

A heap is a data structure that allows you to store a set of elements and keep track of the smallest element in the set (according to a given ordering relation). It is useful for implementing priority queues.

The standard library provides a `std::heap`  that implements a heap class and a `make_heap` function that takes a vector and turns it into a heap. It also provides `push_heap` and `pop_heap` methods that allow you to add and remove elements from the heap. It also provide `std::priority_queue`, a container adapter +for a priority queue.

However those implementations operate on the data vector directly.  In this implementation the heap structure is placed instead on top an existing data vector, whose elements are not moved by the heap operations. In other words, this heap is implemented through an indirection table. It may be useful when you whant to have an heap view of some data in a vector but at the same time you want to keep the position of the elements in the vector unchanged.

In this implementation the data vector is stored as private member and made avaliable, as a contant reference, through the method `data()`. It is constant since the user should not be allowed to change the vector.


## Main heapView operations ##
The heap operations are:
- top(): returns the smallest element in the heap
- pop(): removes the smallest element from the heap, returning a pair contianing the value. Note that the data vector is unchenged!
- add(value): adds an element to the heap. The index of the added element is returned. The data vector increases its size by one: the added element is added to the back of the data vector.
- update(index,value): updates the value of the element in the data vector at the given index. The index is returned.
- remove(index): removes the element at the given index from the heap. The index is returned. The data vector is unchanged!
- operator[](index): returns the value of the element at the given index in the data vector (not is the heap).

** A note ** I have chosen to compose the heap with the vector. I could have chosen aggregation instead, with the internal vector being a reference to an external vector. The advantage of composition is that the heap can be used as a stand alone object. The advantage of aggregation is that the heap can be used to view a subset of an external vector. I have chosen composition because I think that the heap should be a stand alone object, for safety. Having the vector as an external resource could induce the user to change the vector in a way that is not compatible with the heap structure. For example, the user could remove an element from the vector without updating the heap. This would break the heap structure.

# What do I learn here? #
- How to implement a heap data structure
- How to use a heap 
- A use of std::optional
- Use of standard exceptions
- An important algorithm

# A final note: heap and ordered sets #
You can keep track of the smallest elements in a set by using an ordered set. However, the heap is more efficient for this purpose. The ordered set keeps all elements ordered by exploiting a binary data structure. In a heap we use a linear structure and we keep track only of the smallest element. The heap is more efficient when you need to keep track of the smallest element only. The ordered set is more efficient when you need to keep all elements in order.

