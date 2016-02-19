#include "meshGenerators.hpp"
#include <stdexcept>
#include <algorithm>
#include "rk45.hpp"
namespace Geometry
{
  MeshNodes Uniform::operator()() const
  {
    auto const n=this->M_num_elements;
    auto const a= this->M_domain.left();
    auto const b= this->M_domain.right();
    if (n==0) throw std::runtime_error("At least two elements");
    MeshNodes mesh(n+1);
    double h=(b-a)/static_cast<double>(n);
    for(auto i=0;i<n;++i)mesh[i]=a+h*static_cast<double>(i);
    mesh[n]=b;
    return mesh;
    
  }
  
  MeshNodes VariableSize::operator()() const
  {
    // calculate dy/dx = h^-1(x)
    MeshNodes mesh;
    auto const t0=this->M_domain.left();
    auto const T =this->M_domain.right();
    auto const y0=0;
    auto const h_max=(T-t0)/4.;
    auto const h_initial=h_max/100.;
    double constexpr final_error=1e-2;
    int status;
    std::size_t maxSteps = 20000;
    auto solution=
      ODE::rk45(
	   [this](double const & x,double const &){return 1./this->M_h(x);},
	   t0,T,y0,h_initial,h_max,final_error,status,maxSteps);
    auto lastValue=solution.back().second;
    // make it an integer
    std::size_t numElements=std::max(static_cast<std::size_t>(std::round(lastValue)),static_cast<std::size_t>(2));
    if(numElements>M_num_elements) throw std::runtime_error("VariableSize: too many elements");
    // rescale
    using pDouble=std::pair<double,double>;
    double scaling=numElements/lastValue;
    std::transform(solution.begin(),solution.end(),solution.begin(),
		   [scaling](pDouble const & x)
		   {
		     return std::make_pair(x.first,x.second*scaling);
		   }
		   );
    mesh.reserve(numElements+1); // I need to store the nodes
    mesh.push_back(M_domain.left()); // first node
    // Now the internal nodes
    auto pos = solution.cbegin()+1;
    for (std::size_t i=1;i<numElements;++i)
      {
	// find_if finds the first element satisfying the predicate
	auto found = std::find_if(pos,solution.cend(),
				  [i]
				  (pDouble const & value)
				  {
				    return value.second >i;
				  }
				  );
	if (found == solution.end()) throw std::runtime_error("variableSize: Something wrong: cannot find node!");
	pos          = found-1;
	auto xpos1 = pos->first;
	auto ypos1 = pos->second;
	auto xpos2 = found->first;
	auto ypos2 = found->second;
	// Linear interpolation
	// Division by zero should not happen. But.. just in case
	if((ypos2-ypos1)==0) throw std::runtime_error("variableSize: something wring in the spacing function h");
	auto xpos=(xpos1*(ypos2-i)+xpos2*(i-ypos1))/(ypos2-ypos1);
	mesh.push_back(xpos);
      }
    // The last node
    mesh.push_back(M_domain.right());
    return mesh;
  }
  
}
