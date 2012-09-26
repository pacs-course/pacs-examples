#include <iostream>
#include <list>
#include <cmath>
#include <algorithm>

using namespace std;
typedef list<float> List;

void printList( const List& );

struct listPredicate
{
  inline bool operator()( const float& value ) const
  {
    return ( fabs(value) < 4.5 );
  }
};

int main( int argc, char* argv[])
{
  // Decleare the list of 10 elements
  List countList(10);
  
  // Fill the list
  float val = 1;
  for ( List::iterator it = countList.begin(); it != countList.end(); ++it)
    {
      *it = val;
      val *= -1.3;
    }
  
  // Print the list
  printList( countList );

  // Count the number of element in the list with absolute value less then 4.5
  unsigned int N = (unsigned int) count_if( countList.begin(), 
					    countList.end(), 
					    listPredicate() );

  cout << "Number of element with absolute value less then 4.5 is " << N << endl;  
  
  return 0;
}

void printList( const List& printList )
{
  for ( List::const_iterator it = printList.begin(); it != printList.end(); ++it)
    cout << *it << endl;
}
