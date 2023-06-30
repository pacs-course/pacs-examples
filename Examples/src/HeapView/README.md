# An example of a heap #

A heap is a *complete binary tree* data structure with the following properties: the values associated to the children
of a given node are always not "before" the value associated to the node, according to a given ordering relation. If the
ordering relation is less-than, the children have always values not less than the parent value and, consequently, the
root of the heap is the node with the smallest value.

Another important feature of a heap is that the binary tree is always complete, therefore it can be **stored
conveniently in a vector**. It this case, the children of node indexed `i` are
`2i+1` and `2i+2`, and the parent of node indexed `i` is `(i-1)/2` (integer division). If `<` indicates the chosen
ordering relation, we then have that `value(i)>= value((i-1)/2)`. The root of the heap is then stored in the first
element of the vector.

The standard library od C++ provides tools for handling heaps (`std::heap`) where however the value associated to a node
is directly the value stored in the node. This is fine but limiting. Often one wants to create a heap structure over a
given data vector, without operating on the vector.

This can be done by storing in the heap the indexes of the data vector elements, and use the comparision operator not on
the values stored in the heap but on the data elements addressed by the heap elements. In this context it is important
to keep also a reverse map, that I have called `iter`, that given the index `i` of an element of the data vector `data`
returns the index `j` in the heap that contains `i`. In other words you have `heap[iter[i]]=i`. This way, you can easily
operate on data element `i`, since you can recover its position in the heap.

The code on `heapView.hpp` contains the class `apsc::HeapView` that implements exacly what previously described. It
stores the data vector internally, so it is not really a view (maybe one can make one as an alternative by storing a
reference to the data vector). This is however a safety feature since you cannot modify the data elements directly, but
only with the methods `update` and `add`, which keep track of the possible modifications in the heap.

The class is a template class, whose template parameters are the type stored in the data vector and the type of the
callable object that should be used to compare elements in the data vector, defaulted to `std::less` (the default is
then a min-heap, which keeps track of the minimal element).

You have the capabilities of the extracting (with the method `pop()`) the first element of the heap (the smallest
according to the given comparison operator), adding new elements to the data vector and the heap with `add()`, and
remove elements from the heap with `remove()`, beware however that in the case of `remove` *the element is not
eliminated from the data vector* only from the heap.

** A Note ** 
The data is composed in the heap (so it is not really a view) to avoid the user modyfing the data without updating the heap. However, the data vector values are not 
changed by an heap operation, a part from `update` that, as the names says, updates a value. For instance, after `pop()` the value in the data vector is unchanged: the
element is removed from the heap and not from the data

``` c++
    auto [where value]=heap.pop(); // top value removed from the heap
    heap[where]==value; // this is still true! Value still in data
```

One can put back the element in the heap, possibly with changed value, using `update()`: this code is fine,

``` c++
    auto [where value]=heap.pop(); // top value removed from the heap
    heap.update(where,newvalue); // put back the element with a new value.
```


# What do I learn here ? #
 
 - A simple example of a data structure that is not provided by the standard library, but that can be easily implemented
   with the tools provided by the standard library.
 - The implementation of the heap based on a vector, which is a very simple and efficient data structure.
 - The use of `std::optional` to identify leaf nodes in the heap and also keep track of elements deleted from the heap.

# Bibliography #

Any good book on data structures and algorithms. But I suggest Volume 1 of Knuth's *The Art of Computer Programming*.



        
