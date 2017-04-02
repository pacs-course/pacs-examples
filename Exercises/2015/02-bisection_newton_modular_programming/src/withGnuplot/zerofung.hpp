/*!
  \file zerofun.hpp
  \brief Algorithms to find the root of a
  non-linear equation f(x) = 0

  Given a continuous function f(x) on a
  real interval [a, b], such that f(a)f(b) < 0,
  these methods evaluate its zero in (a, b)

*/

#ifndef ZEROFUN_HPP
#define ZEROFUN_HPP 1

#include <string>
#include <cstdlib>
#include <assert.h>
#include <fstream>

#include "rootfinding.hpp"

/*!
  \brief Convergence check

  Given the stopping criterion for an iterative
  algorithm (which is specified in \e check parameter)
  this function compares the increment or the residual
  computed in the last iteration against
  the desired tolerance.

  \param increment norm of the difference between
  two subsequent iterates
  \param residual \f$f(\alpha)\f$ being \f$\alpha\f$
  the approximation of the zero of \f$f\f$
*/
  bool converged(real const & increment, real const & residual,
                 real const & tol, checkT const & check);

/*!
  \name Root finding algorithms

  Some parameters are used in each algorithm.

  - <b>Input Parameters:</b>
    - \e tol tolerance for the approximate solution
    - \e maxit maximum number of iterations
    - \e check type of convergence check
    (INCREMENT or RESIDUAL)
    .
    These are defined \i const as they won't be modified
    inside the procedures. </em></small>

  - <b>Input/Output Parameters:</b>
    - \e nit actual number of iterations performed by the algorithm
    .
    This parameters is overwritten inside the procedure,
    and you don't have to bother on how to initialize it.
  .

*/

/*@{*/
//! Bisection method
/*!
  \param a interval boundary
  \param b interval boundary
  \param f pointer to function f(x)

  Note that \e a and \e b are not defined constant:
  the algorithm needs to update the interval boundaries
*/
real bisection(real a, real b, fctptr const f,
               real const & tol, int const & maxit,
               checkT const & check, int & nit,
               const double & sol_ex, const std::string & file_name);

//! Newton method
/*!
  \param xp initial value
  \param f pointer to function f(x)
  \param df pointer to function derivative [df/dx](x)

  Note that xp is not defined constant: it will contain
  the approximate solution after each iteration
*/
real newton(real xp, fctptr const f, fctptr const df,
            real const & tol, int const & maxit,
            checkT const & check, int & nit,
            const double & sol_ex, const std::string & file_name);

//! Robust method
/*!
  This algorithm combines a low order method with a
  higher order one.
  Starting from an estimate of the solution, obtained
  from bisection method, the Newton method recovers
  an approximate solution within the desired
  tolerance \e tol.

  \param cfratio ratio between tolerance values
  required to the two different algorithms:
  \f[
  tol_{bisection} = cfratio * tol_{newton}
  \f]
*/
real robust(real a, real b,
            fctptr const f, fctptr const df,
            real const & tol, real const & cfratio,
            int const & maxit, checkT const & check,
            int & nit_coarse, int & nit_fine,
            const double & sol_ex, const std::string & file_name);
/*@}*/

#endif
