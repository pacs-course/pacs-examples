# Two examples of expression templates #

Expression templates are a rather complex metaprogramming technique, normally used in linear algebra libraries such as Eigen or Armadillo++ (but not only: one of the examples in this directory is about numerical integration), to satisfy the following requirements:

* Preserve the expressiveness of operators, so that you can write expressions such as `A*B+C` for vectors, matrices, or even polynomials. In other words, this applies to mathematical objects that support an algebra;
* Maximize efficiency and reduce memory usage by delaying the evaluation of an expression until it is actually needed.

For instance, with expression templates you may be able to write the sum of three vectors (or more) as

```
x = a + b + c;
```

but the final code produced by the compiler actually does

```
for (int i....)
  x[i] = a[i] + b[i] + c[i]
```

without creating unnecessary temporaries (and possibly also unrolling the loop if it is short). Details can be found in the bibliography below.

In this directory you will find:

  * `Algebra`: a vector class in which algebraic operations are optimized by using expression templates.
  
  * `Integral`: an example of a quadrature rule made more efficient by expression templates. Note, however, that these implementations are "static", and therefore less flexible and general than the classes in `QuadratureRule` (but probably more efficient).


# What do you learn from these examples? #
These examples show how to use template metaprogramming techniques to preserve both expressiveness and efficiency at the same time. It is, however, a complex technique. Not for the end user (in fact, it simplifies things for them), but certainly for the developer. Another drawback is that compiler error messages can be difficult to interpret, because they become even more counter-intuitive than usual.


*Note: you benefit from expression templates only if you compile with optimization enabled.*

# Bibliography #
- J. Hardtlein, C. Pfaum, A. Linke, C++.H. Wolters. *Advanced expression templates programming* Comp.Visual.Sci, 13:59-68, 2010 doi:10.1007/s00791-009-0128-2
- D.A. Di Pietro, A. Veneziani. *Expression templates implementation of continous and discontinous Galerkin methods*, Comp.Visual.Sci, 12-421-436, 2009, doi:10.1007/s00791-008-0117-x
- `https://en.wikipedia.org/wiki/Expression_templates`
