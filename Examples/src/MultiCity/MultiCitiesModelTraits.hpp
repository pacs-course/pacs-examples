/*
 * MultiCitiesModelTraits.hpp
 *
 *  Created on: Apr 12, 2020
 *      Author: forma
 */

#ifndef EXAMPLES_SRC_MULTICITY_MULTICITIESMODELTRAITS_HPP_
#define EXAMPLES_SRC_MULTICITY_MULTICITIESMODELTRAITS_HPP_
#include "Eigen/Dense"
#include <functional>
namespace apsc
{
  namespace multicity
  {
    template<int NumCities>
    struct MultiCityModelTraits
    {
      //! The type of variables for the solver for population
       using PopulationVariableType=Eigen::Matrix<double,NumCities,NumCities>;
       //! The type of variables for the solver for epidemic model
       using EpidemicVariableType=Eigen::Matrix<double,2*NumCities,NumCities>;
       //! The forcing term type for population model
       using PopulationForcingTermType=std::function<PopulationVariableType (const double &, const PopulationVariableType &)>;
       //! The forcing term type for epidemic model
        using EpidemicForcingTermType=std::function<EpidemicVariableType (const double &, const EpidemicVariableType &)>;
        //! The type for a Vector
       using VectorType = Eigen::Matrix<double,NumCities,1>;
       //! The size of a block
       using BlockType = Eigen::Matrix<double,NumCities,NumCities>;
    };
  }
}



#endif /* EXAMPLES_SRC_MULTICITY_MULTICITIESMODELTRAITS_HPP_ */
