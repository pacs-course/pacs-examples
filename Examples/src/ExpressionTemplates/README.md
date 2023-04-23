# Two examples of use of expression templates #

Expression templates is a rather complex metaprogramming technique normally used in linear algebra libraries (like Eigen or Armadillo++) (but not only, one of the examples in this directory is about numerical integrations) to satisfy the following requirements

* Maintain the expressibility of operators: use expressions like A*B+C also with verctore or matrices (or maybe... polynomials). In other words, for all mathematical entities that have an algebra;
* Maximize efficiency and memory footprint by delaying the computation of the expression only when needed.

For instance, with expression template you may be able to write the sum of three vectors (or more) as

```
x = a + b + c;
```

but the final, compiler produced, code actually does

```
for (int i....)
  x[i] = a[i] + b[i] + c[i]
```

without creating useless temporaries (and maybe also unrolling the loop if it is short). You find the details in the given bibliography.

In this directory we have:

  * `Algebra`. a class of vectors where algebraic operation are
  optimized by using expression templates
  
  * `Integral`: an example of quadrature rule made more efficient by expression templates. Note however that they are "static", less
  flexible and general than the classes in QuadratureRule (but probably more efficient).


#What do you learn with these examples?#
How to use template metaprogramming techniques to maintain high expressibility and efficiency at the same time. It is a complex technique however. Not for the final user (actually it simplifies things for the final user!), but certainly for the developer.  Another drowback is that compiler error messages can be difficul to interpret because they become even more counter-intuitive than usual. 


*Note: remember that you may gain efficiency with expression template only if you compile with optimization!*

# Bibliography #
- J. Hardtlein, C. Pfaum, A. Linke, C.H. Wolters. *Advanced expression templates programming* Comp.Visual.Sci, 13:59-68, 2010 doi:10.1007/s00791-009-0128-2
- D.A. Di Pietro, A. Veneziani. *Expression templates implementation of continous and discontinous Galerkin methods*, Comp.Visual.Sci, 12-421-436, 2009, doi:10.1007/s00791-008-0117-x
- `https://en.wikipedia.org/wiki/Expression_templates`
