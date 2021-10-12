#include <cstdlib>
#include <fstream>
#include <iostream>
/*!  @brief A simple example of file streams.

  @details File streams
  are steams connected to a file. All methods of an iostream are inherited
  by a file stream.
 */
int
main()
{
  std::ofstream file{"data.txt"};
  if(file.fail())
    {
      std::cerr << " Cannot open output file" << std::endl;
      std::exit(1);
    }
  int    i = 10;
  double v = 61.9;
  file << i << " " << v << std::endl;
  file.close();
  std::ifstream ifile("data.txt");
  if(ifile.fail())
    {
      std::cerr << " Cannot open input file" << std::endl;
      std::exit(2);
    }
  int    j;
  double w;
  ifile >> j >> w;
  if(ifile.fail())
    {
      std::cerr << " Something wrong in input file" << std::endl;
      std::exit(3);
    }
  ifile.close();
}
