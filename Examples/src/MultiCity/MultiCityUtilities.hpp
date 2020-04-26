/*
 * MutiCityUtilities.hpp
 *
 *  Created on: Apr 24, 2020
 *      Author: forma
 */

#ifndef EXAMPLES_SRC_MULTICITY_MULTICITYUTILITIES_HPP_
#define EXAMPLES_SRC_MULTICITY_MULTICITYUTILITIES_HPP_
#include "MultiCityEpidemic.hpp"
#include "Eigen/Eigenvalues"
#include "MultiCityPopulation.hpp"
namespace apsc
{
  namespace multicity
  {
    /*!
     * Computes the spectral ratio of an Eigen Matrix
     * @tparam M The matrix type. Any real Eigen Matrix
     * @param m The matrix
     * @return The spectral ratio
     */
    template<class M>
    double spectralRatio(const M& m)
    {
      Eigen::VectorXcd eivals = m.eigenvalues();
      return eivals.array().abs().maxCoeff();
    }
    /*!
     *  Computes the R0 value for each city using the formula
     *  of the article (hoping to have understood it)
     * @tparam EModelData The type of EdidemicData
     * @param N The current population
     * @param t The current time
     * @return A vector containing the R0 for each city
     */
    template<class EModelData>
    auto computeR0(EModelData const & data ,
		   typename MultiCityModelTraits<EModelData::NCities,EModelData::NVar>::PopulationVariableType const & N,
		   double t)
    ->typename EModelData::VectorType
    {
      using BlockType= typename EModelData::BlockType;
      using VectorType = typename EModelData::VectorType;
      constexpr int NCities=EModelData::NCities;
      BlockType Vtemp;
      BlockType V;
      BlockType F;
      VectorType result;
      VectorType g = data.g(t);
      VectorType k = data.k(t);
      for (int q=0;q<NCities;++q)
	{
	  // Hope to have understood. Paper is messy!
	  V.fill(0.0);
	  // extract row of r
	  VectorType rRow=data.r(t).row(q).transpose();
	  // add term gamma + d and
	  // fill diagonal of V
	  V.diagonal() = rRow.array() +data.gamma(t)*data.d(t);
	  // correct q row
	  V.row(q) = - rRow.transpose();
	  // correct q columns
	  VectorType mCol=data.m(t).col(q);
	  mCol[q]=-1.0;
	  // gm term
	  mCol.array()*=-g.array()*mCol.array();
	  // Diagonal
	  V(q,q) += data.gamma(t)+data.d(t);
	  // The Vii matrix of the paper is ready
	  VectorType Np=apsc::multicity::Np(N);
	  F.fill(0.0);
	  for (int j=0; j<NCities;++j)
	    {
	       VectorType kNbetajq= data.beta(t,j).col(q);
	       kNbetajq *=k[q]/Np[q];
	       kNbetajq.array()*=N.col(j).array();
	       F.col(j)=kNbetajq;
	    }
	    BlockType M = F* V.inverse();
	    result(q)=spectralRatio(M);

	}
      return result;
    }

  }
}



#endif /* EXAMPLES_SRC_MULTICITY_MULTICITYUTILITIES_HPP_ */
