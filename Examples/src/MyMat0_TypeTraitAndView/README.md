#Example of matrix that can store objects of arbitrary type#

The stored type must be default and copy/move constructible and copy/move
assignable.

Note the use of type traits to disable methods that do not make sense for
non-arithmetic types.

Matrix is set to zero when initialized or resized, only for arithmetic
types.

I have also implemented a view to have the transpose of an existing
matrix, without having to construct the transpose. Note that I had to
redefine all public methods, so that now operate as if the matrix was
the transpose of the original one.
