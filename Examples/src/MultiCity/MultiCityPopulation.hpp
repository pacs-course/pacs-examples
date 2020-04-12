/*
 * MultiCityPopulation.hpp
 *
 *  Created on: Apr 12, 2020
 *      Author: forma
 */

#ifndef EXAMPLES_SRC_MULTICITY_MULTICITYPOPULATION_HPP_
#define EXAMPLES_SRC_MULTICITY_MULTICITYPOPULATION_HPP_
#include <array>
#include "MultiCitiesModelTraits.hpp"
namespace apsc
{
  namespace multicity
  {
    template<int NumCities>
        struct MultiCityPopulationVariables
        {
          //! The type of variables for the RK solver
          using VariableType= typename MultiCityModelTraits<NumCities>::PopulationVariableType;
          //! The type for a Vector
          using VectorType = typename MultiCityModelTraits<NumCities>::VectorType;
          //! Adjourn current population in each city
          void computeNp()
          {
            Np=N.rowwise().sum();
          }
          /*!
           * Initializes N by setting as a diagonal matrix with Nr as diagonal
           */
          void initializeN(){
            N = Eigen::DiagonalMatrix<double,NumCities,NumCities>(Nr);
            computeNp();
          }
          /*!
           * compute total population (for check)
           *
           * @return an array containing the tatal population summed as the sum of resident and the sum of mobility matrix N
           */
          std::array<double,2> totalPop()
          {
            return {N.sum(),Nr.sum()};
          }
          VariableType N;
          VectorType Np;
          VectorType Nr;
        };
    //! A general template for initialization`
      template<int NumCities>
      struct InitializePopulation
      {
	static void initialize(MultiCityPopulationVariables<NumCities> &);
      };
      //! The two city case of the article
      struct initialize2Cities: public InitializePopulation<2>
      {
	static void initialize(MultiCityPopulationVariables<2> & p)
	{
	  p.Nr<<2500,2500;
	  p.initializeN();
	}
      };
  }
}




#endif /* EXAMPLES_SRC_MULTICITY_MULTICITYPOPULATION_HPP_ */
