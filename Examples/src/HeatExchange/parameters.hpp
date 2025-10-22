/*!
 * @file parameters.hpp
 * @brief Defines the parameters structure for the Heat Exchange example.
 *
 * This header contains the definition of the `parameters` struct, which holds
 * all the configurable parameters for the heat exchange simulation, including
 * solver settings, physical dimensions, material properties, and boundary conditions.
 *
 * The struct is designed as an aggregate, allowing for brace initialization and
 * structured bindings. An overloaded stream insertion operator is provided for
 * convenient output of parameter values.
 *
 * @author 	Luca Formaggia
 * @date 	2019
 */
#ifndef HH_Parameters_HH
#define HH_Parameters_HH
#include <iosfwd>
/*!
 * A structure holding the parameters
 *
 * It is an aggregate, you can use structured binding and brace initialization
 */

struct parameters
/**
 * @brief Parameters for the heat exchange simulation.
 *
 * @details This structure contains all the configurable parameters used in the heat exchange problem,
 * including solver settings, physical dimensions, material properties, and boundary conditions.
 *
 * Members:
 * - int itermax: Maximum number of iterations for the Gauss-Seidel solver.
 * - double toler: Tolerance for the stopping criterion in the iterative solver.
 * - double L: Length of the bar.
 * - double a1: First longitudinal dimension.
 * - double a2: Second longitudinal dimension.
 * - double To: Dirichlet boundary condition (initial temperature).
 * - double Te: External temperature.
 * - double k: Thermal conductivity of the material.
 * - double hc: Convection coefficient.
 * - int M: Number of elements in the discretization.
 * - int solverType: Type of solver to use (0 for direct, 1 for Gauss-Seidel).
 */
{
  //! max number of iteration for Gauss-Siedel
  int itermax = 1000000;
  //! Tolerance for stopping criterion
  double toler = 1.e-8;
  //! Bar length
  double L = 40.0;
  //! First longitudinal dimension
  double a1 = 4.0;
  //! Second longitudinal dimension
  double a2 = 50.;
  //! Dirichlet condition
  double To = 46.;
  //! External temperature
  double Te = 20.;
  //! Conductivity
  double k = 0.164;
  //! Convection coefficient
  double hc = 200e-6;
  //! Number of elements
  int M = 100;
  //! type of solver (0 direct, 1 Gauss Siedel)
  int solverType = 0;
};
//! Prints parameters
std::ostream &operator<<(std::ostream &, const parameters &);
#endif
