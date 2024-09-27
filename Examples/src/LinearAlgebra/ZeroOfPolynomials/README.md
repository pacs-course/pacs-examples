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

### Bibliographic References on Newton-Horner Algorithm

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
   Lunedì 30 Settembre 2024, ore 16.30, in Sala Napoleonica, Palazzo Brera, in Via Brera 28, Milano, il Professore 

Michael Struwe, ETH Zurich  

parlerà su: Old and New Results on the Plateau Problem

Abstract: 
The question whether any closed embedded curve in 3-dimensional space bounds a surface of least area is a classical problem in geometric analysis, going back to Lagrange in the 18th century and the Belgian physicist Joseph Plateau in the 19th century. A first break-through was achieved by Douglas and Rado in 1930/31 in the parametric setting for minimal surfaces of the type of the disc. Building on ideas of Caccioppoli and De Giorgi, Federer-Fleming in the 1960's introduced geometric measure theory as an alternative approach to the problem, which through the work of Allard, Almgren, and - finally - Hardt-Simon culminated in a very general existence result for smoothly embedded minimal surfaces spanning any given curve.

In the talk we quickly review these results and then discuss the more recent approach to the problem via the De Giorgi/Modica-Mortola theory of phase transitions proposed in 1990 by Froehlich-Struwe and revisited by Guaraco-Lynch this year. If time permits we will conclude with a view on the speaker's current work on the 'Plateau flow' as a means to canonically decompose a given surface into a collection of minimal surfaces of bounded topological type spanning the given curve."


Te': ore 16:00, atrio della Sala d'Addunanza dell'Istituto Lombardo 

https://sites.google.com/view/lezionileonardesche/home

Write in markup format some bibliographic references on Newton-Horner algorithm for finding the zeros of polynomials

Monica
GPT-4o
Here are some bibliographic references related to the Newton-Horner algorithm for finding the zeros of polynomials, formatted in a markup style:

Copy
### Bibliographic References on Newton-Horner Algorithm

1. **Author**: Ralston, A., & Rabinowitz, P.  
   **Title**: A First Course in Numerical Analysis  
   **Publisher**: McGraw-Hill  
   **Year**: 1978  
   **Description**: This textbook covers a variety of numerical analysis methods, including the Newton-Horner algorithm, providing both theoretical background and practical applications.

2. **Author**: Stoer, J., & Bulirsch, R.  
   **Title**: Introduction to Numerical Analysis  
   **Publisher**: Springer  
   **Year**: 2002  
   **Description**: This book provides a thorough introduction to numerical analysis, including detailed sections on polynomial zero-finding methods such as the Newton-Horner algorithm.

3. **Authors**: Quarteroni, A., Sacco, R., & Saleri, F.  
   **Title**: Numerical Mathematics  
   **Publisher**: Springer  
   **Year**: 2000  
   **Description**: This book offers a comprehensive introduction to numerical mathematics, including a section on polynomial root-finding methods such as the Newton-Horner algorithm. The authors provide both theoretical foundations and practical insights into the application of these methods.


