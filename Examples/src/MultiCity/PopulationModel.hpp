/*
 * PopulationModel.hpp
 *
 *  Created on: Apr 12, 2020
 *      Author: forma
 */

#ifndef EXAMPLES_SRC_MULTICITY_POPULATIONMODEL_HPP_
#define EXAMPLES_SRC_MULTICITY_POPULATIONMODEL_HPP_
#include "MultiCityEpidemic.hpp"
#include "MultiCityPopulation.hpp"
#include "RKFMC.hpp"
namespace apsc
{
  namespace multicity
  {
    template<int NumCities, template<int> class EpiData>
    class PopulationModel
    {
    public:
      using VariableType= typename MultiCityModelTraits<NumCities>::PopulationVariableType;
      using ForcingTermType= typename MultiCityModelTraits<NumCities>::PopulationForcingTermType;
      using VectorType = typename MultiCityModelTraits<NumCities>::VectorType;
      PopulationModel()=default;
      //! get epidemic model
      PopulationModel(EpiData<NumCities> const & d):data_{d}{}
      //! Setters and getters
      //!@{
      const EpiData<NumCities>& getData () const{ return data_;}
      void setData (const EpiData<NumCities> &data){ data_ = data;}
      //!@}
      //! The operator returning the rhs.
      VariableType operator()(const double & t, const VariableType & N)
      {
	VariableType F;
	// Get coefficients at time t
	auto d = data_.d(t);
	auto g = data_.g(t);
	auto r = data_.r(t);
	auto m = data_.m(t);

	VectorType Nr = N.transpose().rowwise().sum();
	F = -r.array()*N.array(); // r N term in 3.b
	VectorType  rNsum = -F.rowwise().sum(); // r N term in 3.a
	for (unsigned int i=0; i<NumCities;++i)
	  {
	    for (unsigned int j=0; j<NumCities;++j)
	      {
		//if (i==j) continue; // skip diagonal (not needed, better let compiler optimize loop)
		F(i,j)+=g(i)*m(j,i)-d*N(i,j); // Eq 3.b
	      }
	    F(i,i)=rNsum(i)-(g(i)+d)*N(i,i) + d*Nr(i); // Eq 3.a
	  }
      return F;
      }
    private:
      EpiData<NumCities> data_;
    };

    template<int NumCities, template<int> class EpiData=MultiCityDataFixed<NumCities>, class RKsolverType=RKFScheme::RK45_t>
    class
    advanceMultiCityPopulation
    {
    public:
      using PopulationModel=PopulationModel<NumCities,EpiData>;
      using VectorType = typename PopulationModel::VectorType;
      using VariableType = typename PopulationModel::VariableType;
      advanceMultiCityPopulation()=delete; // at the moment is not default constructible
      advanceMultiCityPopulation(PopulationModel const & popModel):
      model{popModel},
      RK{RKsolverType{},popModel}
      {};

      template <typename T>
      void setInitialValue(T&& initialValue)
      {
	N0=std::forward<T>(initialValue);
      }
      multicity::RKFResult<MultiCityPopulationVariables<NumCities>>
      advance(double initialTime, double endTime)
     {
       return RK(initialTime, endTime, N0, hinit,tolerance,maxSteps);
     }

     double tolerance{1.e-4};
     double hinit{1.0};
     int maxSteps{1000};
    private:
      MultiCityPopulationVariables<NumCities> N0;
      PopulationModel model;
      RKFMC<RKsolverType,PopulationModel> RK{};
    };
  }
}



#endif /* EXAMPLES_SRC_MULTICITY_POPULATIONMODEL_HPP_ */
