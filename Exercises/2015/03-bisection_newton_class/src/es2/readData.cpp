#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

int main( int argc, char* argv[] )
{
  
  ifstream file;

  // Open the file
  file.open( argv[1], ifstream::in );

  // Check if the file is correctly open
  if ( !file.is_open() )
    {
      cerr << "Unable to open " << argv[1] << " file." << endl;
      return 1;
    }
    
  string lineString;
  vector<double> dataValue;
  unsigned int n(0);
  
  dataValue.reserve( 10 );

  // Read until the file is empty
  while ( !file.eof() )
    {

      // Take the current line
      getline( file, lineString );

      // Check if the current line is a comment line, if so skip to while
      if ( lineString[0] == '%' )
	continue;

      // Find where the numerical data begun
      string::size_type i = lineString.find('=');

      // Take the corrispective substring
      string subLineString = lineString.substr( i + 1 );
      
      // Convert the string to a sstring
      istringstream isString( subLineString );
      
      // Convert and save the data value in the vector<double>
      isString >> dataValue[ n ];

      // Print the value
      cout << dataValue[ n ] << endl;

      // Update the position
      ++n;

    }

  // Close the file
  file.close();

  return 0;

}
