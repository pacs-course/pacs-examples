#include "GetPot"
#include <cmath>
#include <iostream>

#include "Adams_rule.hpp"
#include "Gauss_rule.hpp"
#include "QuadratureRuleAdaptive.hpp"
#include "helperfunction.hpp"
#include "integrands.hpp"
#include "montecarlo.hpp"
#include "numerical_integration.hpp"
using namespace apsc::NumericalIntegration;
using namespace Geometry;

int
    main(int argc, char **argv) Adams_rule.o integrands.o main_integration
      .o numerical_integration.hpp StandardQuadratureRule.hpp Gauss_rule
      .hpp libintegrands.a make.dep numerical_integration.o helperfunction
      .cpp libintegrands.so Makefile QuadratureRuleAdaptive.hpp helperfunction
      .hpp libquadrature.so montecarlo.cpp QuadratureRuleBase.hpp helperfunction
      .o libquadrules.so montecarlo.hpp QuadratureRulePlusError.hpp forma
    @minnie : ~ /
    Work / Didattica / HPC_Courses / Pacs
  - examples / Examples / src / QuadratureRule / baseVersion$
{
  using namespace std;
  FunPoint f = fsincos;

  double a, b;
  int    nint;
  double targetError;
  readParameters(argc, argv, a, b, nint, targetError);
  cout << "Integral from " << a << " to " << b << " on " << nint << " intervals"
       << endl;
  double exactVal = exact(a, b);

  Domain1D domain(a, b);
  Mesh1D   mesh(domain, nint);

  Quadrature s(Simpson{}, mesh);
  Quadrature m(MidPoint{}, mesh);
  Quadrature t(Trapezoidal{}, mesh);
  Quadrature gL(GaussLegendre3p{}, mesh);

  cout << " Now the mesh has " << mesh.numNodes() << " nodes" << endl;

  double approxs = s.apply(f);
  double approxm = m.apply(f);
  cout << "MidPoint=" << approxm << " Trapezoidal=" << t.apply(f)
       << " Simpson=" << approxs << " GaussLegendre3p=" << gL.apply(f)
       << " Exact=" << exactVal << endl;

  // Now with MonteCarlo

  MonteCarlo mcRule;
  // Desired error
  mcRule.setError(1.e-3);
  Quadrature mc(mcRule, mesh);

  auto approxmc = mc.apply(f);
  cout << "MonteCarlo=" << approxmc
       << " error=" << std::abs(exactVal - approxmc) << " Estimated/error="
       << dynamic_cast<MonteCarlo const &>(mc.myRule()).cumulatedError() /
            std::abs(exactVal - approxmc)
       << endl;

  // Now the adaptive
  Quadrature sa(QuadratureRuleAdaptive<Simpson>(targetError, 10000), mesh);
  double     adaptiveResult = sa.apply(f);
  printout(adaptiveResult, exactVal, targetError, "SImpson Adaptive");

  // Now the adaptive
  Quadrature ga(QuadratureRuleAdaptive<GaussLobatto4p>(targetError, 10000),
                mesh);
  adaptiveResult = ga.apply(f);
  printout(adaptiveResult, exactVal, targetError, "Gauss Lobatto Adaptive");
}
