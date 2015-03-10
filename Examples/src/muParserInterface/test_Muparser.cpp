#include "muParserInterface.hpp"
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
    cout<< "Give me an expression possibly contaning variables t, x and y"<<std::endl;
    cout<< "(type quit to finish)"<<std::endl;
    while(expr.size()==0) // to filter out extra carriage returns
      {
	getline(cin,expr);
	if(!cin.good()) printAndExit();
      }
    if(!cin.good()) printAndExit();
    cout<<"You have typed:"<<expr<<endl;
    if(expr=="quit") exit(0);
    cout<<"Give me the values of t, x and y"<<std::endl;
    cin>>t>>x>>y;
    if(!cin.good()) printAndExit();
    try
      {
	p.set_expression(expr);
	coor={{x,y}};
	cout<<"Result: "<<p(t,coor.data())<<std::endl;
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
}
