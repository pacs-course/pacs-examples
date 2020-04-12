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
namespace apsc
{
  namespace multicity
  {
    template<int NumCities, template<int> class EpiData>
    class PopulationModel
    {
    public:
      using VariableType= typename MultiCityPopulationVariables<NumCities>::VariableType;
      using ForcingTermType= typename MultiCityModelTraits<NumCities>::PopulationForcingTermType;
      using VectorType = typename MultiCityPopulationVariables<NumCities>::VectorType;
      PopulationModel()=default;
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
  }
}



#endif /* EXAMPLES_SRC_MULTICITY_POPULATIONMODEL_HPP_ */
