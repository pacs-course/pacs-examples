#include "bcContainer.hpp"
#include "muParserInterface.hpp"
#include <string>
#include <map>

//! @file An example of use of a container for boundary conditions

// Normally this stuff is on a separate files or are read from files
// I can use a lambda
auto inflow= [](double const t, double const * coord){return t*coord[0];};
// I can use a function object
class setValue
{
public:
  setValue(double v):value(v){};
  
  double operator()(const double, double const *)
  {
    return value;
  }
private:
  double value;
};

//I can use a function

double myfun(double const t, double const * coord)
{
  return (coord[0]+coord[1]);
};

int main()
{
  using namespace FEM;
  std::map<BCType,std::string> dictionary;
  dictionary[Dirichlet]="Dirichlet";
  dictionary[Neumann]="Neumann";

  BCContainer bcContainer;
  BCBase bc1(Dirichlet,"Wall 1",zerofun);
  // add a list of identifiers
  bc1.set_entities(std::vector<unsigned int>{0,4,8});
  bcContainer.push_back(bc1);
  // I can use the helper function to ass a b cond
  {
    auto & bc2=addToBCContainer(Neumann,"Far Field 1",myfun,bcContainer);
    bc2.set_entities( std::vector<unsigned int>{1,2,3} );
    // bc2 is now eliminated. I prefer it because the container data may be reallocated
  }
  addToBCContainer(Dirichlet,"Inflow",inflow,bcContainer);
  bcContainer.back().set_entities(std::vector<unsigned int>{9,10,11});
  addToBCContainer(Neumann,"Far Field 1",myfun,bcContainer);
  bcContainer.back().set_entities(std::vector<unsigned int>{12,13,14});
  setValue ten(10.0);
  addToBCContainer(Dirichlet,"Outflow",ten,bcContainer);
  bcContainer.back().set_entities(std::vector<unsigned int>{15,16});
  addToBCContainer(Dirichlet,"back", MuParserInterface::muParserInterface("x+t"),bcContainer);
  bcContainer.back().set_entities(std::vector<unsigned int>{20});
  double t=10;
  std::array<double,2> point{{0.,3.}};
  std::cout<<"Content of bc container organized by type"<<std::endl;
  
  try{
    for (auto typ : dictionary)
      {
	auto list=extractBCWithType(bcContainer,typ.first);
	std::cout<<"******Condition type: "<<typ.second<<std::endl;
	for (auto i : list)
	  {
	    i->showMe();
	    std::cout<<std::endl;
	    std::cout<<"Value at point "<<i->apply(t,point.data())<<std::endl<<std::endl;
	  }
      } 
  }
  catch (mu::Parser::exception_type &e)
    {
      std::cout<<"******ERROR in evaluating expression******"<<std::endl;
      MuParserInterface::printMuException(e);
      std::cout<<"******************************************"<<std::endl;
      std::cout<<std::endl;

    }
}
