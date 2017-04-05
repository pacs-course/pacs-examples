#include <iostream>
#include <list>
#include <iterator>
#include <algorithm>

using namespace std;

typedef list<float> List;

int main( int argc, char* argv[])
{
  // Decleare the list of 10 elements
  List countList(10);
  
  // Fill the list
  float val = 1;
  for ( auto & i: countList )
  {
      i = val;
      val *= -1.3;
  }
  
  // Print the list
  ostream_iterator<float> out_it (cout,"\n");
  copy( begin(countList), end(countList), out_it );

  // Count the number of element in the list with absolute value less then 4.5
  unsigned int N = (unsigned int) count_if( begin(countList), 
					    end(countList), 
      []( const float & value ) { return fabs(value) < 4.5; } );

  cout << "Number of element with absolute value less then 4.5 is " << N << endl;  
  
  return 0;
}

