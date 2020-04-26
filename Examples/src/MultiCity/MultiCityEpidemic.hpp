/*
 * MultiCity.hpp
 *
 *  Created on: Apr 10, 2020
 *      Author: forma
 */

#ifndef EXAMPLES_SRC_MULTICITY_MULTICITYEPIDEMIC_HPP_
#define EXAMPLES_SRC_MULTICITY_MULTICITYEPIDEMIC_HPP_
#include <array>
#include <iostream>
#include "MultiCitiesModelTraits.hpp"
namespace apsc
{
  namespace multicity
  {
    //! The variable set used for the epidemic model
    template<int NumCities,int NumVariables=2>
        using MultiCityEpidemicVariables =typename MultiCityModelTraits<NumCities,NumVariables>::EpidemicVariableType;

    /*! It is a wrapper that defines the variable necessary to solve the dynamic system for the multicity model
     *
     * We refer to the article A multi-city epidemic model, by J.Arino and P. van den Driessche
     *
     * The variable S (susceptibles) and I (infectious) are stored in a 2N X N Eigen Matrix
     * and can be extracted singularly. It can be extended to other models
     *
     * @tparam NumCities Number of cities
     */
    template<int NumCities>
    struct MultiCityEpidemicVariablesSISProxy
    {
      //! The type of variables for the RK solver
      using VariableType=typename MultiCityModelTraits<NumCities,2>::EpidemicVariableType;
      //! The type of S and I
      using BlockType = typename MultiCityModelTraits<NumCities,2>::BlockType;
      //! The type for a Vector
      using VectorType = typename MultiCityModelTraits<NumCities,2>::VectorType;
      //! Constructor takes a reference to the Variable Set
      MultiCityEpidemicVariablesSISProxy (VariableType & SI):SI{SI}{};
      //! @defgroup mainvariables Functions returning the main variables S and I for the SIS model
      //! @{
      decltype(auto) S(){return SI.template block<NumCities,NumCities>(0,0);}
      decltype(auto) S()const {return SI.template block<NumCities,NumCities>(0,0);}
      decltype(auto) I(){return SI.template block<NumCities,NumCities>(NumCities,0);}
      decltype(auto) I()const {return SI.template block<NumCities,NumCities>(NumCities,0);}
      //@}
      //! This for a future model,, for instance SIR or SEIR
      /*!
       * @tparam VariableSet the set I want to extract
       * @return The extracted set
       */
      template <unsigned int VariableSet>
      decltype(auto) Var(){return SI.template block<NumCities,NumCities>(NumCities*VariableSet,0);}
      //! The number of cities
      static constexpr int NCities= NumCities;
      //! The number of Variable Sets
      static constexpr unsigned NumEquationSet=MultiCityModelTraits<NumCities,2>::NumEquationSet;

    private:
      //! The matrix with the global variables \f$[S,I]^T\f$
      VariableType & SI;

    };

    /*! Data for the model (fixed parameters version)
     *
     * It provides the public interface for all data.
     * The functions returning the date are all time dependent to define the general interface
     * even if here there is no dependence on time
     *
     * I have used the same notation of the article A multi-city epidemic model, by J.Arino and P. van den Driessche
     *
     * @tparam NumCities Number of cities
     *
     */
    template<int NumCities,int NumVariables=2>
    class MultiCityDataFixed
    {
    public:
      using VariableType= typename MultiCityModelTraits<NumCities,NumVariables>::PopulationVariableType;
      using BlockType= typename MultiCityModelTraits<NumCities,NumVariables>::BlockType;
      using VectorType = typename MultiCityModelTraits<NumCities,NumVariables>::VectorType;
      static constexpr int NCities=NumCities;
      static constexpr int NVar=NumVariables;
      //! Time is stored in a double
      using Time = double;
      /*!
       * @param time (not used)
       * @return the return rate
       */
      BlockType r(Time const &)const {return r_;}
      /*!
       * @param time (not used)
       * @return per capita rate of outbound movement
       */
      VectorType g(Time const &) const {return g_;}
      /*!
       *
       * @param time (not used)
       * @return average number of contacts in each city
       */
      VectorType k(Time const&) const {return k_;}
     /*!
       * @param time (not used)
       * @param k the city index
       * @return disease transmission coefficient for city k
       */
      BlockType beta(Time const & ,int k)const {return beta_k[k];}
      /*!
       * @param time (not used)
       * @return recovery rate
       */
      double gamma(Time const &)const {return gamma_;}
      /*!
       * @param time (not used)
       * @return death rate
       */
      double d(Time const &)const {return d_;}
      /*!
       * @param time (not used)
       * @return mobility fractions m
       */
      BlockType m(Time const &)const {return m_;}
      /*!
       * Initialization with article value.
       * Works only if NumCities=2 or 3
       */
      /*!
       * This is an extension to the model of the article. It is the fraction of recovered individuals
       * that are immune. In the SIS model of the article this fraction is 0. All recovered becomes subsceptile again
       * In a SIR model is 1: all recovered are immune. So S + I is not constant anymore (and equal to the immune)
       * @param
       * @return
       */
      double immuneFraction(Time const &) const {return immuneFraction_;}
      //! An utility function
      /*!
       * It gives a reasonable initialization
       */
	void initializeFromArticle();
      // I leave them public for simplicity
      // @todo: make setters
      BlockType r_;
      VectorType g_;
      std::array<BlockType,NumCities> beta_k;
      VectorType k_;
      BlockType m_;
      double gamma_=0.0;
      double d_=0.0;
      double immuneFraction_=0;
    };

    template<>
    void MultiCityDataFixed<2>::initializeFromArticle()
    {
      gamma_=1./25.;
      k_<<1.0, 1.0;
      d_=1./(75*365);
      r_<<0.0, 0.05, 0.05,0.0;
      beta_k[0]<<0.016,0.016,0.016,0.016;
      beta_k[1]<<0.048,0.048,0.048,0.048;
      g_<<0.25,0.15;
      m_<<0.,1.0,1.0,0.;
      immuneFraction_=0;
    }

    template<int NumCities>
    std::ostream& operator<< (std::ostream & out,MultiCityDataFixed<NumCities> mc)
    {
      out<<" N. Cities: "<<NumCities;
      out<<" gamma="<<mc.gamma(0)<<std::endl;
      out<<" r    =\n"<<mc.r(0)<<std::endl;
      out<<" g    ="<<mc.g(0)<<std::endl;
      out<<" d    =\n"<<mc.d(0)<<std::endl;
      out<<" m    =\n"<<mc.m(0)<<std::endl;
      out<<" kappa=\n"<<mc.k(0)<<std::endl;
     for (int i=0;i<NumCities;++i)
	{
	  out<<"beta for city "<<i+1<<std::endl;
	  out<<" beta=\n"<<mc.beta(0,i)<<std::endl;
	}
      return out;
    }

  }
}



#endif /* EXAMPLES_SRC_MULTICITY_MULTICITYEPIDEMIC_HPP_ */
