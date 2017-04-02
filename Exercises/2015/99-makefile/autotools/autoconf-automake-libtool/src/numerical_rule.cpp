#include "numerical_rule.hpp"
#include <cmath>

namespace NumericalIntegration{

  // Costructor
  QuadratureRule::QuadratureRule():
    M_w     (),
    M_n     (),
    M_order (0)
  {} // Costructor

  // Virtual destructor.
  QuadratureRule::~QuadratureRule()
  {} // Virtual destructor.

  // Costructor
  Simpson::Simpson()
  {
    // Fill the weights.
    M_w.push_back(1./3);
    M_w.push_back(4./3);
    M_w.push_back(1./3);

    // Fill the nodes.
    M_n.push_back(-1.0);
    M_n.push_back(0.0);
    M_n.push_back(1.0);
  } // Costructor

  // Costructor
  MidPoint::MidPoint()
  {
    // Fill the weight.
    M_w.push_back(2.0);

    // Fill the node.
    M_n.push_back(0.0);
  } // Costructor

  // Costructor
  Gauss4Pt::Gauss4Pt()
  {
    // Fill the weights
    M_w.push_back(1./6.);
    M_w.push_back(5./6.);
    M_w.push_back(5./6.);
    M_w.push_back(1./6.);

    // Fill the nodes.
    M_n.push_back(-1.);
    M_n.push_back(-sqrt(5.)/5.);
    M_n.push_back(sqrt(5.)/5.);
    M_n.push_back(1.);
  } // Costructor

}
