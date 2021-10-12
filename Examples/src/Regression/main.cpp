/*
 * main.cpp
 *
 *  Created on: Jan 24, 2019
 *      Author: forma
 */
#include "RegressionSolver.hpp"
#include <MSECostFunction.hpp>
#include <PolynomialRegressionEvaluator.hpp>
// If I have CppNumericalSolvers installed
#ifndef NOCPPSOLVER
#include "CostFunctionProxyCppNumSolver.hpp"
#include "cppoptlib/meta.h"
#include "cppoptlib/problem.h"
#include "cppoptlib/solver/bfgssolver.h"
#include "cppoptlib/solver/gradientdescentsolver.h"
#endif

#include <iostream>
#include <random>

int
main()
{
  using namespace LinearAlgebra;
  // The libary for linear algebra to use. At the moment only EIGEN is
  // implemented You need to use constexpr since we need to pass it as tempalte
  // argument and a template valued argument must be a constant expression NOTE
  // however thaat, for compatibility with previous versions of C++ it is
  // possible to just declare libraryToUse const. And it can be passed as
  // tempalte argument all the same.

  constexpr LinearAlgebra::LinearAlgebraLibrary libraryToUse =
    LinearAlgebra::EIGEN;
  // I use polynomials of degree 5.
  std::size_t                                   n = 5;
  PolynomialMonomialBasisFunction<libraryToUse> modelBasis{n};
  using Model = PolynomialRegressionEvaluator<
    PolynomialMonomialBasisFunction<libraryToUse> >;
  Model model(modelBasis);
  using Vector = LinearAlgebra::RegressionTraits<libraryToUse>::Vector;
  Vector trainx(6);
  Vector trainy(6);
  Vector testx(2);
  Vector testy(2);
  // train data follow a quadratic with a small perturbation, just for test
  // Probably the result will be an overfitting....
  // you may try something different!
  // I use the random device to give entropy!
  std::random_device r;
  // the random engine is initialised with the random device.
  // Each run will be different since I will be using a different realization.
  std::default_random_engine rand(r());
  // I assume error nomally distributed with mean zero and small variance
  std::normal_distribution<double> normaldist(0.0, 1e-2);
  for(int i = 0; i < trainx.size(); ++i)
    {
      trainx(i) = static_cast<double>(i);
      trainy(i) = static_cast<double>(i * i) + normaldist(rand);
    }

  LinearRegressionSolverMSE<Model> L(model);
  auto                             param = L.solve(trainx, trainy);
  std::cout << " With the QR factorization\n Parameters:\n";
  std::cout << param << std::endl;
  MSECostFunction<Model> J(model);
  std::cout << "Training MSE error:\n";
  std::cout << J.eval(trainx, trainy, param) << std::endl;
  testx << 4.5, 8.0; // these are just made up to tast the code!
  testy << 4.5 * 4.5 + normaldist(rand), 8.0 * 8.0 + normaldist(rand);
  std::cout << "Generalization error:\n";
  std::cout << J.eval(testx, testy, param) << std::endl;
  std::cout << "Values at the test points:\n";
  std::cout << model.eval(testx[0], param) << " " << model.eval(testx[1], param)
            << std::endl;

#ifndef NOCPPSOLVER
  using Problem = CostFunctionProxyCppSolver<MSECostFunction<Model> >;
  Problem f(J, trainx, trainy);
  // cppoptlib::GradientDescentSolver<Problem> solver;
  cppoptlib::BfgsSolver<Problem> solver;
  // Initial guess, seet to zero
  Vector guess = Vector::Zero(model.size());
  solver.minimize(f, guess);
  std::cout << "\nResults from optimization\nParameters:\n " << guess
            << std::endl;
  std::cout << "Training MSE error:\n";
  std::cout << "J= " << J.eval(trainx, trainy, guess) << std::endl;
  std::cout << "Generalization error:\n";
  std::cout << J.eval(testx, testy, guess) << std::endl;
  std::cout << "Values at the test points:\n";
  std::cout << model.eval(testx[0], guess) << " " << model.eval(testx[1], guess)
            << std::endl;

#endif
}
