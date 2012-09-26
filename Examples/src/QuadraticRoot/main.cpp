/*
 * main.cpp
 *
 *  Created on: 22 Aug 2011
 *      Author: formaggia
 */
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include "QuadraticRoot.hpp"
int main(){
  using namespace std;
  double x1,x2;
  cout<< "I will compute the root of ax^2+b^x+c"<<endl;
  cout<< "This version uses floating point type of size "<<sizeof(Real)<<" bytes"<<endl;
  Real a(0);
  Real b(1);
  Real c(0);
  std::pair<Real,Real> result;
  cout.setf(ios::scientific); // use scientific float format
 TryAgain:
  while (true)
    {
      cout<<"Give me a, b and c (all zeros to end)"<<endl;
      // Beware: this is not safe. If you give in input data
      // not convertible to Reals you get unpredictable result
      // A better implementation uses getline and istringstreams
      cin >> a>> b>>c;
      if(cin.fail())
	{
	  cerr<<"Wrong input: try again"<<endl;
	  // If the input stream is screwed up
	  // it is difficult to bring it back to a good state.
	  // A possibility is to read everithing up to the end of the 
	  // budder and try again. First clear the state flags!
	  cin.clear();
	  while (!cin.eof())cin.ignore();
	  //! Clear again so that eof flag is reset.
	  cin.clear();
	  // Normally you should try to avoid goto's but this
	  // is one of the few cases where a goto makes the code 
	  // easier to read.
	  goto TryAgain;
	}
      // C++11 feature: max may take a parameter list!
      if (std::max({a,b,c}) != 0.0)
	{
	  try
	    {
	      // Compute by less stable method
	      result = quadraticRoot_simple(a,b,c);
	      cout<< " With naive method:"<<endl;
	      cout<<"x1="<<result.first<<" x2="<<result.second<<endl;
	      // Residual is always computed in double precision
	      x1=static_cast<double>(result.first);
	      x2=static_cast<double>(result.second);
	      cout<<"Residuals: "<< a*x1*x1+b*x1+c<<", "<<
		a*x2*x2+b*x2+c<<endl<<endl;
	      // Compute with more stable method
	      cout<< " With more stable method:"<<endl;
	      result=quadraticRoot(a,b,c);
	      cout<<"x1="<<result.first<<" x2="<<result.second<<endl<<endl;
	      // Residual is always computed in double precision
	      x1=static_cast<double>(result.first);
	      x2=static_cast<double>(result.second);
	      cout<<"Residuals: "<< a*x1*x1+b*x1+c<<", "<<
		a*x2*x2+b*x2+c<<endl<<endl;
	    }
	  catch (negativeDiscriminant error)
	    {
	      cerr<<error.what()<<endl;
	      //exit(1);
	    }
	}else
	{
	  exit(0);
	}
    }
}
