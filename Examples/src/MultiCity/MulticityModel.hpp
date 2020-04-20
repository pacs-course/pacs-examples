/*
 * MulticityModel.hpp
 *
 *  Created on: Apr 19, 2020
 *      Author: forma
 */

#ifndef EXAMPLES_SRC_MULTICITY_MULTICITYMODELADVANCE_HPP_
#define EXAMPLES_SRC_MULTICITY_MULTICITYMODELADVANCE_HPP_
#include "PopulationModel.hpp"
#include "EpidemicModel.hpp"
#include <iostream>
#include <tuple>
namespace apsc
{
  namespace multicity
  {
    /*!
     * A struct that encapsulate the result
     */
    template<int NumCities>
    struct Result
    {
      using EpidemicVariableType= typename MultiCityModelTraits<NumCities>::EpidemicVariableType;
      using PopulationVariableType= typename MultiCityModelTraits<NumCities>::PopulationVariableType;
      //! the population at each time
      std::vector<std::tuple<double,PopulationVariableType>> population;
   	//! the epidemic variables at each time
      std::vector<std::tuple<double,EpidemicVariableType>> epidemic;
      bool good=true;
   };
    /*!
     * The class that advances a multicitymodel.
     *
     * It incorporates a model for the population dynamics and a model for
     * epidemic dynamics.
     *
     * It uses a Runge Kutta Fehlberg algorithm for time advance
     *
     * @tparam NumCities N. of cities
     * @tparam PModel Population model
     * @tparam EModel Epidemic Models
     * @tparam RKsolverType Runge kutta scheme
     */
    template <
    int NumCities,
    typename PModel,
    typename EModel,
    typename RKsolverType=RKFScheme::RK45_t>
    class MultiCityModelAdvance
    {
    public:
      static_assert(PModel::NCity==EModel::NCity,"Population and epidemic model must have same n. of cities");
      /*!
       * Main variables
       *{*/
      using EpidemicVariableType= typename MultiCityModelTraits<NumCities>::EpidemicVariableType;
      using PopulationVariableType= typename MultiCityModelTraits<NumCities>::PopulationVariableType;
      using PopulationForcingTermType= typename MultiCityModelTraits<NumCities>::PopulationForcingTermType;
      using EpidemicForcingTermType= typename MultiCityModelTraits<NumCities>::EpidemicForcingTermType;
      using VectorType = typename MultiCityModelTraits<NumCities>::VectorType;
      using BlockType = typename MultiCityModelTraits<NumCities>::BlockType;
      /*!{*/
/*!
  *  A struct (aggregate) that encapsulates the data for the solver
  */
      struct solverData{
	//! Initial time
	double tInitial=0.0;
	//! Final time
	double tFinal=10.0;
	//! Number of steps for which output is produced
	unsigned int numSteps=5;
	//! Max number of iteration for the RK solver
	unsigned int maxSteps=50;
	//! Initial value for population
	PopulationVariableType N_Initial;
	//! Initial value for Epidemic model
	EpidemicVariableType E_Initial;
	//! Desired error per time step (Population)
	double populationTolerance=1.e-3;
	//! Desired error per time step (Epidemic)
	double epidemicTolerance=1.e-3;
      };

	/*!
	 * The class is not default constructible
	 * EpidemicModel must take a PopulationVariable as reference
	 *
	 * @todo It can be made copy-constructible, but then I need to set the epidemic data
	 * and for that I need a constructor in EpidemicModel taking only the reference
	 * to the PopulationVariable it stores as reference!
	 */
	MultiCityModelAdvance()=delete;
	/*!
	 * Constructor taking epidemic model data
	 * @tparam E Type of Epidemic model data
	 * @param data The epidemic data
	 * @note The epidemic data must be consistent with that of the epidemic and
	 * population models. If not the constructor fails
	 */
	template<template <int> class ED>
	MultiCityModelAdvance(ED<NumCities> const & data):
	Ncurrent{},
	populationModel{data},
	epidemicModel{data,Ncurrent},
	initialData{},
	RKP{apsc::RKFScheme::make_RK<RKsolverType>(),populationModel},
	RKE{apsc::RKFScheme::make_RK<RKsolverType>(),epidemicModel}
	{}

	template <template <int> class EpiData>
	void loadEpidemicData(EpiData<NumCities> const & ed)
	{
	  populationModel.setData(ed);
	  epidemicModel.setData(ed);
	  RKP.set_ButcherArray(apsc::RKFScheme::make_RK<RKsolverType>());
	  RKP.set_function(populationModel);
	  RKE.set_ButcherArray(apsc::RKFScheme::make_RK<RKsolverType>());
	  RKE.set_function(epidemicModel);
	}
	/*!
	 * Advances the model from tInitial to tFinal in numSteps
	 *
	 * @return an object of type MultiCityModelAdvance::result
	 */
	Result<NumCities> advance() const;
	// Left public for simplicity
	//! The current value of the population variable.
	//! Bound to the reference in EpidemicModel
	//! @note mutable because changed during iterations
	mutable PopulationVariableType Ncurrent;
	//! The model for the population dynamics
	PModel populationModel;
	//! The model for the epidemic dynamics
	//! @note mutable because changed during iterations
	mutable EModel epidemicModel;
	//! The struct with the initial data
	solverData initialData;
    private:
	//! RK solver for population
	RKFMC<RKsolverType,PModel> RKP;
	//! RK solver for epidemics
	RKFMC<RKsolverType,EModel> RKE;
    };

    /*!
     *
     * @tparam RKR The type of Runge Kutta Fehlberg scheme
     * @param out an output stream for the logging
     * @param res the current result returned by the RKF solver
     */
    template<typename RKR>
    void logger(std::ostream& out, RKR const & res)
    {
      out<<"Time="<<res.time.back()<<" Failed="<<std::boolalpha<<res.failed;
      out<<" Inner iterations="<<res.y.size()<<" Estimated error= "<<res.estimatedError;
      out<<" L1 norm="<<res.y.back().array().abs().sum()<<std::endl;
    }

    /*!
     * For outputting results
     * @tparam Res MultiCityModelAdvance result type
     * @param out output stream
     * @param res a result
     * @return the stream
     */
    template <int NumCities>
    std::ostream & operator<<(std::ostream& out, Result<NumCities> const & res)
    {
      // The format of the file is given by: first the epidemic and then the population
      out<<"#Epidemic Data"<<std::endl;
      out<<res.epidemic.size()<<"# N. of steps"<<std::endl;
      for (auto const & [time, value] : res.epidemic )
	  out<<time<<"\n"<<value<<"\n";
      out<<"#Population Data"<<std::endl;
      out<<res.population.size()<<"# N. of steps"<<std::endl;
      for (auto const & [time, value] : res.population )
	  out<<time<<"\n"<<value<<"\n";
      out<<std::flush;
      return out;
    }
  }// end namespace multicity
}

// Implementation of advance()
template<int NumCities,
    typename PModel,
    typename EModel,
    typename RK>
    typename apsc::multicity::Result<NumCities>
  apsc::multicity::MultiCityModelAdvance<NumCities, PModel, EModel, RK>::advance () const
  {
    using namespace apsc::multicity;
    // Get data, so I have it handy
    auto const & tInitial= this->initialData.tInitial;
    auto const & tFinal= this->initialData.tFinal;
    auto const & numSteps= this->initialData.numSteps;
    auto const & ptol= this->initialData.populationTolerance;
    auto const & etol= this->initialData.epidemicTolerance;
    auto const &  N0 =this->initialData.N_Initial;
    auto const &  E0 =this->initialData.E_Initial;
    auto const & maxSteps = this->initialData.maxSteps;
    // the step
    double hstep = (tFinal -tInitial)/numSteps;
    // I start with 1/2
    double hinit=hstep/2;
    // The current time
    double time = tInitial;
    // Variables to advance
    typename PModel::VariableType N;
    typename EModel::VariableType E;
    // result object
    Result<NumCities> res;
    // reserve space in advance, for efficiency
    res.population.reserve(numSteps+1);
    res.epidemic.reserve(numSteps+1);
    // Save initial data
    res.population.emplace_back(std::make_tuple(tInitial,N0));
    res.epidemic.emplace_back(std::make_tuple(tInitial,E0));
    // Set intitial values
    N = N0;
    E=  E0;
    bool good= true;
    // Iterate
    for (std::size_t iter=0; iter<numSteps;++iter)
      {
	double t0 = time;
	time +=hstep;
	auto resp = RKP(t0, time, N, hinit,ptol,maxSteps);
	std::clog<<"Population step. ";
	apsc::multicity::logger(std::clog,resp);
	res.population.emplace_back(resp.lastValues());
	good = good && (!resp.failed);
	N    = resp.y.back(); //update
	// I use updated population to move epidemic
	// of course other choices are possible
	this->Ncurrent=resp.y.back();
	auto rese = RKE(t0, time, E, hinit,etol,maxSteps);
	std::clog<<"Epidemic step.   ";
	apsc::multicity::logger(std::clog,rese);
	res.epidemic.emplace_back(rese.lastValues());
	good = good && (!rese.failed);
	E    = rese.y.back(); // update
	// Update

      }
    res.good=good;
    return res;
  }

#endif /* EXAMPLES_SRC_MULTICITY_MULTICITYMODEL_HPP_ */
