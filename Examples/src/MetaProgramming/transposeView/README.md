#An example of a view (called also proxy)#

A view provides a different semantic to an *existing object*. For instance, in this example we take an object of type Matrix
which can be any Matrix type whose elements can be accesses with a call operator with two integer arguments representing the row and column indexes, respectively ( `m(i,j)` is the value in location `i,j`).

The view allow to access the matrix with the index exchanged, so it provides its transpose.

The object in which a view operates is normally stores as a (lvalue) reference, so a view is normally not default constructible: it needs an object to be "attached to". Remember that a (lvalue) reference must always bind to an object, we cannot have "dangling references".

The basic set up of a view is simple. A main difficulty arises if you wish to treat cont object properly. Here we show some rather sophisticated way of achieving this using type traits.

