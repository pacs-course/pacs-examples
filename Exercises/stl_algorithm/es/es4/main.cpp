#include <iostream>
#include <set>
#include <numeric>
using namespace std;
typedef set<int> Set;

void printSet( const Set& );

struct setFunctor
{
  inline int operator() ( const int& first, const int& second ) const
  {
    return ( first + 3* second );
  }
};

int main(int argc, char* argv[])
{
  // Declare the set
  Set sumSet;

  // Fill the set
  int N = 10;
  for ( int i = 0; i < N ; ++i )
      sumSet.insert( i );
   
  // Print the set
  printSet( sumSet );

  // Define the starting value
  int startingValue = 0;
  
  // Accumulate all the values
  int sum = accumulate( sumSet.begin(), sumSet.end(), startingValue, setFunctor() );
  
  // Print the sum
  cout << "Sum " << sum << endl;

  return 0;
}

void printSet( const Set& setPrint)
{
  for( Set::const_iterator it = setPrint.begin(); it != setPrint.end(); it++ )
    cout << *it << endl;
}
