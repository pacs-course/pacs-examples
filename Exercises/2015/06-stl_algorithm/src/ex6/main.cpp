#include <iostream>
#include <map>
#include <cstdlib>

using namespace std;

typedef double (*FunPoint)( const double&, 
			    const double&, 
			    const double& );

// Boundary function
double dirichlet_1 ( const double& x, const double& y, const double& z )
{
  return x+2.*y-z*z;
}

double dirichlet_2 ( const double& /*x*/, const double& y, const double& /*z*/ )
{
  return 2.*y-6.;
}

int main()
{
  typedef map<unsigned int, FunPoint> bcMap;

  // Map to store all the boundary conditions
  bcMap boundaryMap;

  // Single boundary condition pair, usefull for insertion
  bcMap::value_type pairBC;

  // Insert the first pair
  boundaryMap[1] = *dirichlet_1;

  // Insert the second pair
  boundaryMap[5] = *dirichlet_2;

  // Extract the iterator of the function with label 5
  bcMap::iterator iteratorBC = boundaryMap.find( 5 );
  
  // Check if the function is present
  if ( iteratorBC == boundaryMap.end() )
  {
      cerr << " Boundary not found " << endl;
      abort();
  }
  
  // Evaluate the function
  cout << "Value " << iteratorBC->second(1, 2, 3) << endl;  

  return 0;
}

