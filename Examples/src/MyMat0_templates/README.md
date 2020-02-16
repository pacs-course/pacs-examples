
#A simple class for full matrices.#

An example on how to switch from row major to column major ordering using the
type-tag strategy. 

To recall how it works, type-tag is a technique that expoits function
overloading to select the function/method to be called. Since
overloading applies on argument types we need to find a way to convert
a value (typically an integer or an enumerator) into a type, so that
different values correspond to different types. For integral values
the standard library introduces `std::integral_constan<T,Value>` for
the purpose. But here, I preferred to do it directly (it is very
simple).

The type of storage is defined by an enumerator, whose type is
`StoragePolicySwitch` and may take the values `COLUMNMAJOR` or `ROWMAJOR`. Then, to convert it to a value you simply define


    template< StoragePolicySwitch S>
    struct StorageType{};

an empy template stuct!.
Then, the function that returns the index in the data storage correspinding to the matrix indexes i and j is provided by two overloaded private methods

    
    size_type getIndex(size_type const i, size_type const j, StorageType<ROWMAJOR>)
    size_type getIndex(size_type const i, size_type const j, StorageType<COLUMNMAJOR>)

The storage policy is a template parameter of the matrix, so, for simplicity, 
the actual function to select the index is another overloading

    size_type getIndex(size_type const i, size_type const j) const
    {
    return getIndex(i,j,StorageType<storagePolicy>());
    }

which, in turn, calls the correct one!

In this example, some utilities are introduced to implement matrix matrix multiplication, in the standard or in a more cache friendly way, again by exploiting 
type tag.

The main uses the utility chrono to do some timing. Compile with `DEBUG=no` if you want that the timings be meaningful.

*BEWARE:* the test uses also the blas library to show how it can be
implemented to speed-up some operatio!  The
[BLAS](https://en.wikipedia.org/wiki/Basic_Linear_Algebra_Subprograms)
library is a highly optimised library for basic linear operations.  It
is normally available in any system since it is used by many
applications. If not, check it is surely among the packages
installable in your system, you need the developer version.

*You may have to correct the local Makefile.inc so that it addresses
the `libblas` library installed in your system!* If you do not have the
blas, compile with BLAS=no, for instance compiling with

**Note** Of course, the selection of the type of indexing could have been made also with an if statement


    size_type getIndex(size_type const i, size_type const j) const
    {
        if(storagePolicy==ROWMAJOR)
        //do something
        else
        // do something else
    }


but in this case the selection is made **run-time** and since the computation of the index will be made very often (everytime the Matrix is accessed!) this is 
inefficient (run-time if statement are expensive). Tag-dispatching is instead resolved **compile-time**.

**A final Note** C++17 has introduced compile-time if statement (`if constexpr`) which may replace the use of tag dispatching:

    
    size_type getIndex(size_type const i, size_type const j) const
    {
        if constexpr (storagePolicy==ROWMAJOR)
        //do something
        else
        // do something else
    }

This `if` is resolved **compile-time**, so in this case we could have avoided tag-dispatching. But only since C++17. And there are still cases where tag-dispatching is better and, afre all, is not so complicated.



