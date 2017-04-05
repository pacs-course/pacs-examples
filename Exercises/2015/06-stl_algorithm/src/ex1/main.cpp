#include <iostream>
#include <complex>
#include <vector>
#include <cstdlib>
#include <algorithm>

using namespace std;

typedef complex<int> Complex;

struct sortPredicate
{
  bool operator()( const Complex& first, const Complex& second ) const
  {
    return (norm(first) > norm(second));
  }
};

void printVect( const vector<Complex>& );

int main(int argc, char* argv[])
{
  // The number of complex
  unsigned int N = 10;

  // Create the vector with zero length
  vector<Complex> vectCompl(0);

  // Reserve N complex
  vectCompl.reserve( N );

  mt19937 gen;
  uniform_int_distribution<> dist(1,100);

  // Fill the vector
  for ( unsigned int i = 0; i < N; ++i )
  {
      double real = ( dist(gen) );
      double imag = ( dist(gen) );
      vectCompl.push_back( Complex( real, imag ) );
  }

  // Print the vector
  printVect( vectCompl );

  // Sort the vector
  sort( begin(vectCompl), end(vectCompl), sortPredicate() );

  cout << "Ordered sequence" << endl;

  // Print the sorted vector
  printVect( vectCompl );
  

  return 0;
}

void printVect( const vector<Complex>& vect )
{
  for ( auto i: vect)
    cout << i.real() << " + i * " << i.imag() << endl;  
}
