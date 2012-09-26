#include <iostream>
#include <set>
#include <algorithm>

using namespace std;
typedef set<int> Set;

void fillSet( Set& , const int&, const unsigned int& );

void printSet( const Set& );

int main( int argc, char* argv[] )
{
  // Declare two sets
  Set first;
  Set second;

  Set unionSet;
  Set intersectionSet;  

  // Fill the first set
  fillSet( first, 1, 10 );

  // Fill the second set
  fillSet( second, 8, 7 );

  // Print the first set
  printSet( first );

  // Print the second set
  printSet( second );

  // Union of the sets
  set_union( first.begin(), first.end(), 
	     second.begin(), second.end(),
	     inserter(unionSet, unionSet.begin() ) );

  printSet( unionSet );

  // Intersection of the sets
  set_intersection( first.begin(), first.end(),
		    second.begin(), second.end(),
		    inserter(intersectionSet, intersectionSet.begin() ) );
  
  printSet( intersectionSet );

  return 0;
}

void fillSet( Set& fill, const int& start, const unsigned int& N )
{
  for( unsigned int i = 0; i<N; ++i)
      fill.insert( start + i);
}

void printSet( const Set& setPrint )
{
  for( Set::const_iterator it = setPrint.begin(); it != setPrint.end(); it++ )
    cout << *it << endl;
  cout << endl;
}
