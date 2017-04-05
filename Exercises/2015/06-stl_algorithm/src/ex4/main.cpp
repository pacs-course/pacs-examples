#include <iostream>
#include <set>
#include <numeric>

using namespace std;

typedef set<int> Set;

int main(int argc, char* argv[])
{
  // Declare the set
  Set sumSet;

  // Fill the set
  int N = 10;
  for ( int i = 0; i < N ; ++i )
      sumSet.insert( i );
   
  // Print the set
  for( auto i: sumSet )
	  cout << i << endl;

  // Define the starting value
  int startingValue = 0;
  
  // Accumulate all the values
  int sum = accumulate( begin(sumSet),
		        end(sumSet),
			startingValue,
			[] ( const int& sum, const int& i ) { return sum + 3 * i; } );
  
  // Print the sum
  cout << "Sum " << sum << endl;

  return 0;
}

