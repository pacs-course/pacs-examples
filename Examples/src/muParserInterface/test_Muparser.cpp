#pragma GCC diagnostic ignored "-Wsuggest-override"
#include "muParserInterface.hpp"
#include "muParserXInterface.hpp"
#include "string_utility.hpp"
#include<iostream>
#include<string>

void printAndExit()
{
  std::cout<<"Something wrong with input"<<std::endl;
  std::exit(1);
}
int main()
{
  using namespace std;
  using namespace MuParserInterface;
  std::string expr;
  muParserInterface p;
  double x,y,t;
  array<double,2> coor;
  while (true)
    {
    cout<< "Give me an expression possibly containing variables t, x and y"<<std::endl;
    cout<< "(type quit to finish)"<<std::endl;
    while(expr.size()==0) // to filter out extra carriage returns
      {
	getline(cin,expr);
	if(!cin.good()) printAndExit();
      }
    if(!cin.good()) printAndExit();
    cout<<"You have typed:"<<expr<<endl;
    if(expr=="quit") break;
    cout<<"Give me the values of t, x and y"<<std::endl;
    cin>>t>>x>>y;
    Utility::cleanStream(cin);
    if(!cin.good()) printAndExit();
    try
      {
	p.set_expression(expr);
	coor={{x,y}};
	cout<<"Result: "<<p(t,x,y)<<std::endl;
	expr.clear();
      }
    catch (mu::Parser::exception_type &e)
      {
	cout<<"******ERROR in evaluating expression******"<<std::endl;
	printMuException(e);
	cout<<"******************************************"<<std::endl;
	cout<<endl;
	expr.clear();
      }
    }
  std::cout<< "Now with array of dimension three\n";
  muParserXInterface<3> mx;
  expr.clear();
  while (true)
    {
      std::array<double,3> x;
      cout<< "Give me an expression possibly contaning x[0], x[1] and x[2]"<<std::endl;
      cout<< "(type quit to finish)"<<std::endl;
      while(expr.size()==0) // to filter out extra carriage returns
        {
          getline(cin,expr);
          if(!cin.good()) printAndExit();
        }
      if(!cin.good()) printAndExit();
      cout<<"You have typed:"<<expr<<endl;
      if(expr=="quit") exit(0);
      cout<<"Give me the values of x[0], x[1] and x[2]"<<std::endl;
      cin>>x[0]>>x[1]>>x[2];
      Utility::cleanStream(cin);
      if(!cin.good()) printAndExit();
      std::cout<<"Parsing expression"<<std::endl;
      mx.set_expression(expr);
      std::cout<<"Done. Now evaluating"<<std::endl;
      cout<<"Result: "<<mx(x)<<std::endl;
      expr.clear();
    }
  
}
