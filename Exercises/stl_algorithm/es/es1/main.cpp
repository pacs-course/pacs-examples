#include <iostream>
#include <complex>
#include <vector>
#include <cstdlib>
#include <algorithm>
using namespace std;

typedef complex<int> Complex;

struct sortPredicate
{
  inline bool operator()( const Complex& first, const Complex& second ) const
  {
    return (norm(first) > norm(second));
  }
};

void printVect( const vector<Complex>& );

int main(int argc, char* argv[])
{
  // The number of complex
  unsigned int N = 5;

  // Create the vector with zero length
  vector<Complex> vectCompl(0);

  // Reserve N complex
  vectCompl.reserve( N );

  // Fill the vector
  for ( unsigned int i = 0; i < N; ++i )
    {
      double real = ( rand() % 100 );
      double imag = ( rand() % 100 );
      vectCompl.push_back( Complex( real, imag ) );
    }

  // Print the vector
  printVect( vectCompl );

  // Sort the vector
  sort( vectCompl.begin(), vectCompl.end(), sortPredicate() );

  cout << "Ordered sequence" << endl;

  // Print the sorted vector
  printVect( vectCompl );
  

  return 0;
}

void printVect( const vector<Complex>& vect )
{
  for ( vector<Complex>::const_iterator it = vect.begin(); it != vect.end(); it++)
    cout << it->real() << " + i * " << it->imag() << endl;  
}
