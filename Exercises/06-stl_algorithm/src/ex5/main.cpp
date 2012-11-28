#include <iostream>
#include <set>
#include <algorithm>

using namespace std;

typedef set<int> Set;

int main( int argc, char* argv[] )
{
  // Declare two sets
  Set firstSet;
  Set secondSet;

  Set unionSet;
  Set intersectionSet;  

  // Fill the first set
  int start = 1;
  generate_n( inserter(firstSet, begin(firstSet)), 10, [&start](){ return start++;} );

  // Fill the second set
  start = 8;
  generate_n( inserter(secondSet, begin(secondSet)), 7, [&start](){ return start++;} );

  // Print the first set
  cout << "{ ";
  for( auto i: firstSet )
      cout << i << " ";
  cout << "}" << endl;

  // Print the second set
  cout << "{ ";
  for( auto i: secondSet )
      cout << i << " ";
  cout << "}" << endl;

  // Union of the sets
  set_union( begin(firstSet), end(firstSet), 
	     begin(secondSet), end(secondSet),
	     inserter( unionSet, begin(unionSet) ) );

  cout << "{ ";
  for( auto i: unionSet )
      cout << i << " ";
  cout << "}" << endl;

  // Intersection of the sets
  set_intersection( begin(firstSet), end(firstSet), 
	            begin(secondSet), end(secondSet),
		    inserter( intersectionSet, begin(intersectionSet) ) );
  
  cout << "{ ";
  for( auto i: intersectionSet )
      cout << i << " ";
  cout << "}" << endl;

  return 0;
}

