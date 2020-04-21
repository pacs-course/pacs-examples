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
    /*!
     * Traits for the multiciti epidemic mode.
     *
     * Taylored for the SIS model. It may be easily changed for more complex models
     *
     * @tparam NumCities the number of cities
     */
    template<int NumCities, int NumEpidVariables=2>
    struct MultiCityModelTraits
    {
       //! The number of cities
       static constexpr unsigned int NCities = NumCities; // the SIS model has 2
       /*!
        * The number of sets of variables in the epidemic model
        * 2 for the SIS model it should be changed for other models
        */
       static constexpr unsigned int NumEquationsSet = NumEpidVariables; // the SIS model has 2
      //! The type of variables for the solver for population
       using PopulationVariableType=Eigen::Matrix<double,NumCities,NumCities>;
       //! The type of variables for the solver for epidemic model
       using EpidemicVariableType=Eigen::Matrix<double,NumEquationsSet*NumCities,NumCities>;
       //! The forcing term type for population model
       using PopulationForcingTermType=std::function<PopulationVariableType (const double &, const PopulationVariableType &)>;
       //! The forcing term type for epidemic model
        using EpidemicForcingTermType=std::function<EpidemicVariableType (const double &, const EpidemicVariableType &)>;
        //! The type for a Vector
       using VectorType = Eigen::Matrix<double,NumCities,1>;
       //! The size of a block
       using BlockType = Eigen::Matrix<double,NumCities,NumCities>;
    };

    // Utilities
    //! Namespace for internals, not general use
    namespace internals
    {
      /*!
       *
       * @tparam NumCities Number of cities
       * @tparam Counter Set of variable being considered
       * @tparam MaxCounter Dimensions (internally determined
       * @tparam T Type of the matrix where blocks are added
       * @tparam B Type of the matrix block to be added
       * @tparam Args Type of other matrix blocks to be added
       * @param R The matrix where blocks are added
       * @param add The matrix to be added
       * @param args The matrices to be added
       */
      template<int NumCities, int Counter, int MaxCounter, typename T, typename B,
               typename... Args>
      void join_impl(T & R, B const& add, const Args&... args)
      {
        R.template block<NumCities,NumCities>(Counter*NumCities,0) =
          add;
        if constexpr (Counter<MaxCounter)
          {
            internals::join_impl<NumCities,Counter+1,MaxCounter>(R, args...);
          }
      }
    }// end namespace internals
    /*!
     * Joins together matrices of type BlockType to produce a global matrix
     *
     * @tparam NumCities N. of cities
     * @tparam Args The type matrix blocks to be joined, of dimension of a BlockTye (Numcities x NumCities)
     * @param args The matrices to be joined (in number NumEquationsSet)
     * @return A matrix of type EpidemicVariableType (size NumEquationsSet*NumCities x NumCities)
     */
    template<int NumCities, typename... Args>
    auto join(const Args&... args)
    {
      Eigen::Matrix<double,(sizeof...(args))*NumCities,NumCities> R;
      internals::join_impl<NumCities,0,sizeof...(args)-1>(R,args...);
      return R;
    }
    /*!{*/
    /*!
     *
     * @tparam NumCities Number of cities (compulsory)
     * @tparam Index Equations set to extract
     * @tparam Matrix Type of Eigen Matrix from which extract the set
     * @param M Th Eigen Matrix from which extract the set
     * @return The extracted block
     *
     * @note this is the non const version: allows writing on the matrix
     */
    template<int NumCities, int Index, class Matrix>
    decltype(auto) extract(Matrix & M)
    {
      return M.template block<NumCities,NumCities>(Index*NumCities,0);
    }
    /*!
     *
     * @tparam NumCities Number of cities (compulsory)
     * @tparam Index Equations set to extract
     * @tparam Matrix Type of Eigen Matrix from which extract the set
     * @param M Th Eigen Matrix from which extract the set
     * @return The extracted block
     *
     * @note this is const version: only for getting values.
     */
    template<int NumCities, int Index, class Matrix>
    decltype(auto) extract(Matrix const & M)
    {
      return M.template block<NumCities,NumCities>(Index*NumCities,0);
    }
    /*!{*/



  }// end namespace
} //end namespace



#endif /* EXAMPLES_SRC_MULTICITY_MULTICITIESMODELTRAITS_HPP_ */
