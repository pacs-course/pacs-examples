# Zeros of polynomials with the Newton Horner algorithm #

Newton-Horner algorithms exploits the properties of synthetic division (see annexed LaTeX file for a description)
to find a zero of a polynomial efficiently, using Newton algorithm.

Using a deflation procedure it may be possible to find **all** zeroes.

Since we are guaranteed toe have n zeroes (counting their multiplicity) for a polynomial of order n only if we work in the complex plane, the utility given here works always with complex numbers, even if the original polynomial has real coefficients.

Here the polynomial is described just by its coefficients. This makes the utitlity rather general. Indeed it can interface with the class in `src/Polynomials`, since an object of that class returns the coefficients in a form compatible with the function `polyRoots` introduced here.

The function `polyRoots` rely on a class, called `polyHolder`, provided in the same file, that stores the coefficients and provides the methods used by `polyRoot`. It is an helper class, which in principle should be kept in an internal namespace, since it is not of general use.

However, I have expoited the class to present some methods for computing derivatives as a point (via synthetic division) and to calculate the derivative of a polynomial, using algorithms different from that included in the 
class in `Polynomials/polinomials.hpp`.

Note that when computing the "next zero" I start from the complex conjugate of the last found zero. This way, if the coefficients are real, I have the chance to fall directly on a different zero, saving time.

Indeed, for polynomial with real coefficient one can implement a different deflation technique to account for "conjugate zeroes". But I have avoided it for the sake of simplicity.

Newton-Horner has no global convergence property, particularly for polynomials in the complex plane. One can ameliorate it by a backtracking technique, as in the `Newton` example. 

Deflation may also introduce  round-off error. So consider this code only as a interesting starting point.

#What do I learn here ?#
-An iteresting algorithm for polynomial zeroes.
- A use of `mutable`: the coefficients of the associated polynomial are stored when synthetic division is performed to compute polinomial derivatives at a point. But the method to compute the derivative is "morally const": I expect it to be callable also on constant `polyHolder` objects. 
