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
      static int constexpr NCity=NumCities;
      using EpidemicDataType=EpiData<NumCities>;
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
      VariableType operator() (const double & t, const VariableType & N) const
      {
	VariableType F;
	// Get coefficients at time t
	auto d = data_.d(t);
	auto g = data_.g(t);
	auto r = data_.r(t);
	auto m = data_.m(t);

	//VectorType Nr=N.rowwise().sum();
	VectorType Nr=apsc::multicity::Nr(N);
	VariableType gM=m*g.asDiagonal();
	F = -r.array()*N.array(); // r N term in 3.b
	VectorType  rNsum = -F.rowwise().sum(); // r N term in 3.a
	for (unsigned int i=0; i<NumCities;++i)
	  {
	    for (unsigned int j=0; j<NumCities;++j)
	      {
		//if (i==j) continue; // skip diagonal (not needed, better let compiler optimize loop)
		F(i,j)+=gM.coeff(j,i)*N.coeff(i,i)-d*N.coeff(i,j); // Eq 3.b
	      }
	    F(i,i)=rNsum(i)-(g(i)+d)*N.coeff(i,i) + d*Nr(i); // Eq 3.a
	  }
      return F;
      }
    private:
      EpiData<NumCities> data_;
    };

    template<int NumCities, template<int> class EpiData, class RKsolverType=RKFScheme::RK45_t>
    class
    advanceMultiCityPopulation
    {
    public:

      using PopModel=PopulationModel<NumCities,EpiData>;
      using VectorType = typename PopModel::VectorType;
      using VariableType = typename PopModel::VariableType;
      advanceMultiCityPopulation()=default;
      advanceMultiCityPopulation(PopModel const & popModel):
      model{popModel},
      RK{apsc::RKFScheme::make_RK<RKsolverType>(),popModel}
      {};

      template <typename T>
      void setInitialValue(T&& initialValue)
      {
	N0=std::forward<T>(initialValue);
      }
      multicity::RKFResult<MultiCityPopulationVariables<NumCities>>
      advance(double const & initialTime, double const & endTime)
     {
       return RK(initialTime, endTime, N0, hinit,tolerance,maxSteps);
     }
     template <class P=PopModel>
     void set_PopulationModel(P&& pop)
     {
       model= std::forward<P>(pop);
       RK.set_ButcherArray(apsc::RKFScheme::make_RK<RKsolverType>());
       RK.set_function(model);
     }
     double tolerance{1.e-4};
     double hinit{1.0};
     unsigned int maxSteps{1000};
    private:
      MultiCityPopulationVariables<NumCities> N0;
      PopModel model;
      RKFMC<RKsolverType,PopModel> RK{};
    };
  }
}



#endif /* EXAMPLES_SRC_MULTICITY_POPULATIONMODEL_HPP_ */
