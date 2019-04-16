/*
 * writeDensityFunction.cpp
 *
 *  Created on: Apr 16, 2019
 *      Author: forma
 */
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include "GetPot.hpp"
#include <algorithm>
namespace {
  void printHelp(std::ostream & out=std::cout)
  {
    out<<"Synopsis: writeDensityFunction -f filename\n";
    out<<"Produces a file <filename_dist.dat> with the density function";
  }
}
//! @brief produces a file with the density function distribution of given data
//!
//! @detail The data is provided in a file with format
//! N
//! x1 x2 .. XN
//!
//! and is specified with the opiton -f=file
int main(int argc, char * argv[])
{
  GetPot   cl(argc, argv);
  if( cl.search(2, "-h", "--help") )
    {
        printHelp();
        return 0;
      }
    // check for file
    std::string filename = cl.follow("NOFILE","-f");
    // Test if you have not provided a file
    if (filename=="NOFILE")
      {
        std::cerr<<"You need to specify a input file";
        printHelp(std::cerr);
        return 1;
      }
    std::ifstream inputFile(filename);// open file
    // Check if file is ok
    if (inputFile.fail())
      {
        std::cerr<<"Input file "<<filename<<" not existent or not readable\n";
        printHelp(std::cerr);
        return 1;
      }
    // create outfile stripping extension
    auto outFile=filename.substr(0, filename.find_last_of(".")) + std::string("_dist.dat");
    int N;
    inputFile>> N;
    if (inputFile.fail())
      {
        std::cerr<<"First line of input file "<<filename<<" not in the right format\n";
        printHelp(std::cerr);
        return 1;
      }
    std::vector<double> values(N);
    for (auto & i:values)inputFile>>i;
    // Close file after having read all data.
    inputFile.close();
    // Now build the distribution
    std::sort(values.begin(),values.end());
    using DistData=std::pair<double,double>;
    std::vector<DistData> dist;
    dist.reserve(N);
    auto res = values.begin();
    // Compute the probability function
    for (auto v:values)
      {
        // Very complicated, it can be done in a simpler way using a set
        // but it is a good reminder of useful algorithms.
        res = std::find_if(res,values.end(),[&v](const double & x){return x>v;});
        double distValue=std::distance(values.begin(),res)/static_cast<double>(N);
        dist.push_back(std::make_pair(v,distValue));
      }
    // I have to eliminate repetitions in dist
    // remember that unique moves the repeated elements at the end
    // and returns the iterator to the end of the range without repetitions
    // I need a predicate since the test is mede only on the data value not on the value of the
    // distribution function
    auto newEnd = std::unique(dist.begin(),dist.end(),[](const DistData & x, const DistData & y){return x.first==y.first;});
    std::ofstream outFileStream(outFile);
    if (outFileStream.fail())
      {
        std::cerr<<"Output file"<<outFile<<" cannot be opened\n";
        return 2;
      }
// The probability density function is the derivative, which I compute with centered
// finite differences
    for(auto i=dist.begin()+1;i<newEnd-1;++i)
      {
        auto delta   = (i+1)->second-(i-1)->second;
        auto h       = (i+1)->first- (i-1)->first;
        outFileStream<< i->first <<" "<<delta/h<<" "<<i->second<<"\n";
      }
    outFileStream.close();
}



