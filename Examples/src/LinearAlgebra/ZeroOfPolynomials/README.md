# Zeros of Polynomials with the Newton-Horner Algorithm #

The Newton-Horner algorithm exploits the properties of synthetic division
(see the attached LaTeX file for a description) to find a zero of a polynomial
efficiently by means of Newton's method.

Using a deflation procedure, it is possible to find **all** zeros.

Since we are guaranteed to have `n` zeros, counting multiplicity, for a
polynomial of degree `n` only if we work in the complex plane, the utility
provided here always works with complex numbers, even if the original
polynomial has real coefficients.

Here the polynomial is described only by its coefficients. This makes the
utility rather general. In particular, it can interface with the class in
`src/Polynomials`, since an object of that class returns the coefficients in a
form compatible with the function `polyRoots` introduced here.

The function `polyRoots` relies on a class called `PolyHolder`, provided in the
same file, which stores the coefficients and provides the methods used by
`polyRoots`. It is a helper class which, in principle, should be kept in an
internal namespace since it is not of general use.

However, here I also used the class to develop methods for computing
derivatives at a point, via synthetic division, and to calculate the
coefficients of a polynomial derivative using an algorithm different from the
one included in the file `Polynomials/polinomials.hpp`.

Note that, when computing the "next zero", the algorithm starts from the
complex conjugate of the last found zero. In this way, if the coefficients are
real, there is a chance of landing directly on a different zero, saving time.

Indeed, for polynomials with real coefficients one can implement a different
deflation technique to account for conjugate zeros. However, I avoided it here
for the sake of simplicity.

Newton-Horner has no global convergence property, particularly for polynomials
in the complex plane. One can improve it by adding a backtracking technique, as
in the `Newton` example. That would be another possible extension.

Deflation may also introduce round-off errors. You can find ways to address
this issue in the specialized literature. Therefore, consider this code only as
an interesting starting point.


# What Do I Learn Here? #
- An interesting algorithm for finding polynomial zeros.
- A use of `mutable`: the coefficients of the associated polynomial are stored
  when synthetic division is performed to compute polynomial derivatives at a
  point. However, the method that computes the derivative is "morally const",
  so I expect it to be callable also on constant `PolyHolder` objects.
- The use of `std::complex` to represent complex numbers. In particular, the use of `std::conj` to compute the complex conjugate of a complex number.
- The use of `std::vector` to store the coefficients of a polynomial. In particular, the use of `std::vector::insert` to insert a new element at the beginning of a vector.
- The use of `std::vector::erase` to remove an element from a vector.

### Bibliographic References on the Newton-Horner Algorithm

1. **Author**: Acton, F. S.  
   **Title**: Numerical Methods That Work  
   **Publisher**: Harper & Row  
   **Year**: 1970  
   **Description**: This book provides a comprehensive overview of numerical methods, including a detailed explanation of the Newton-Horner method for polynomial root finding.

2. **Author**: Stoer, J., & Bulirsch, R.  
   **Title**: Introduction to Numerical Analysis  
   **Publisher**: Springer  
   **Year**: 2002  
   **Description**: This book provides a thorough introduction to numerical analysis, including detailed sections on polynomial zero-finding methods such as the Newton-Horner algorithm.
   

3. **Author**: Ralston, A., & Rabinowitz, P.  
   **Title**: A First Course in Numerical Analysis  
   **Publisher**: McGraw-Hill  
   **Year**: 1978  
   **Description**: This textbook covers a variety of numerical analysis methods, including the Newton-Horner algorithm, providing both theoretical background and practical applications.

4. **Author**: Stoer, J., & Bulirsch, R.  
   **Title**: Introduction to Numerical Analysis  
   **Publisher**: Springer  
   **Year**: 2002  
   **Description**: This book provides a thorough introduction to numerical analysis, including detailed sections on polynomial zero-finding methods such as the Newton-Horner algorithm.

5. **Authors**: Quarteroni, A., Sacco, R., & Saleri, F.  
   **Title**: Numerical Mathematics  
   **Publisher**: Springer  
   **Year**: 2000  
   **Description**: This book offers a comprehensive introduction to numerical mathematics, including a section on polynomial root-finding methods such as the Newton-Horner algorithm. The authors provide both theoretical foundations and practical insights into the application of these methods.

