# Zeros of polynomials with the Newton-Horner algorithm #

Newton-Horner algorithm exploits the properties of synthetic division (see annexed LaTeX file for a description)
to find a zero of a polynomial efficiently, using Newton algorithm.

Using a deflation procedure it is possible to find **all** zeroes.

Since we are guaranteed to have n zeroes (counting their multiplicity) for a polynomial of order n only if we work in the complex plane, the utility given here works always with complex numbers, even if the original polynomial has real coefficients.

Here the polynomial is described just by its coefficients. This makes the utility rather general. Indeed it can interface with the class in `src/Polynomials`, since an object of that class returns the coefficients in a form compatible with the function `polyRoots` introduced here.

The function `polyRoots` rely on a class, called `polyHolder`, provided in the same file, that stores the coefficients and provides the methods used by `polyRoot`. It is an helper class, which in principle should be kept in an internal namespace since it is not of general use.

However, here I have expoited the class to develop some methods for computing derivatives as a point (via synthetic division) and to calculate the derivative of a polynomial, using an algorithm different from the one included in the file `Polynomials/polinomials.hpp`.

Note that, when computing the "next zero", I start from the complex conjugate of the last found zero. This way, if the coefficients are real, I have the chance to fall directly on a different zero, saving time.

Indeed, for polynomial with real coefficient one can implement a different deflation technique to account for the "conjugate zeroes". But I have avoided it here for the sake of simplicity.

Newton-Horner has no global convergence property, particularly for polynomials in the complex plane. One can ameliorate it by a backtracking technique, as in the `Newton` example (another possible extension).

Deflation may also introduce  round-off error, you can find in the specialised literature ways to address the problem. So consider this code only as a interesting starting point.

# What do I learn here ? #
-An iteresting algorithm for polynomial zeroes.
- A use of `mutable`: the coefficients of the associated polynomial are stored when synthetic division is performed to compute polinomial derivatives at a point. But the method to compute the derivative is "morally const": I expect it to be callable also on constant `polyHolder` objects.
- The use of `std::complex` to represent complex numbers. In particular, the use of `std::conj` to compute the complex conjugate of a complex number.
- The use of `std::vector` to store the coefficients of a polynomial. In particular, the use of `std::vector::insert` to insert a new element at the beginning of a vector.
- The use of `std::vector::erase` to remove an element from a vector.
