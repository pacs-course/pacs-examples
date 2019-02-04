/*
 * ButcherRKF.hpp
 *
 *  Created on: Feb 3, 2019
 *      Author: forma
 */

#ifndef SRC_RK45_BUTCHERRKF_HPP_
#define SRC_RK45_BUTCHERRKF_HPP_
#include<array>
namespace ODE
{
//! The class containing the prototype of a Butcher array for RKF methods
template<unsigned int NSTEP>
struct ButcherArray{
  using Atable=std::array<std::array<double,NSTEP>,NSTEP>;
  ButcherArray(Atable const &a,
               std::array<double,NSTEP> const & b1,
               std::array<double,NSTEP> const & b2,
               double ord):
    A(a),b1(b1),b2(b2),order(ord)
  {
    c.fill(0.0);
    // loop over rows of A
    for (std::size_t i=1;i<NSTEP;++i)
      // Loop over cols of A
      for (auto const & v: A[i]) c[i]+=v;
  }
/*! I store the full array even if only the part below the main diagonal is
 * different from zero. For simplicity
 */
  Atable A;
  //! The b1 coefficient of the Butcher array (lower order)
  std::array<double,NSTEP> b1;
  //! The b2 coefficients of the Butcher array (higher order)
  std::array<double,NSTEP> b2;
  //! The c coefficient of the butcher array
  std::array<double,NSTEP> c;
  //! I need the order to control time steps
  int order;
  //! For simplicity.
  /*!
   * Not necessary since I could estract this information from the array
   * using tuple_size()
   */
  static constexpr unsigned int Nstep=NSTEP;
};

// SOME COMMON RK embedded schemes
//! RK45, the actual RK <Fehlberg scheme
using RK45_t = ButcherArray<6>;
//! Here the object declaration
extern ButcherArray<6> RK45;

using RK23_t = ButcherArray<4>;
extern RK23_t RK23;


}


#endif /* SRC_RK45_BUTCHERRKF_HPP_ */
