/*
 * MultiCityPopulation.hpp
 *
 *  Created on: Apr 12, 2020
 *      Author: forma
 */

#ifndef EXAMPLES_SRC_MULTICITY_MULTICITYPOPULATION_HPP_
#define EXAMPLES_SRC_MULTICITY_MULTICITYPOPULATION_HPP_
#include "Eigen/Dense"
#include <array>
namespace apsc
{
  namespace multicity
  {
    template<int NumCities>
        struct MultiCityPopulationVariables
        {
          //! The type of variables for the RK solver
          using VariableType=Eigen::Matrix<double,NumCities,NumCities>;
          //! The forcing term type
          using ForcingTermType=std::function<VariableType (double const &, VariableType const &)>;
          //! The type for a Vector
          using VectorType = Eigen::Matrix<double,NumCities,1>;
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
  }
}




#endif /* EXAMPLES_SRC_MULTICITY_MULTICITYPOPULATION_HPP_ */
