#include<string>
#include<iostream>
#include<sstream>
#include<string>
#include<fstream>
#include<vector>
/*!
  @brief A simple example of stringstreams
  
  @details A string stream is a stream that used as i/o buffer
  a std::string. It is very useful. We show two usage: the first
  to create a file name that depends on the value of a variable. The
  second, to read a file analysing it line by line (with also the use
  of getline())
 */
int main()
{
  /* I want to create a string, called filename
     of the form data_XX.dat where XX is an integer
     (maybe the iteration number of an iterative method)
  */

  int XX=10;
  // S string stream for output
  std::ostringstream tmp;
  tmp<<"data_"<<XX<<".dat";
  // dump into filename.
  // str() returns the string contained into the sstream
  std::string filename(tmp.str());

  std::cout<<" The file name is "<<filename<<std::endl;

  /* A different use of a string stream.
     I want to read from a file where data 
     is written as
     # Comment: A file that contains some data
      Point1:  0.3 0.4 # acomment
      Point2:  0.5 0.8
  */
  std::ifstream file("datafile.dat");
  std::vector<double> x;
  std::vector<double> y;
  double x1,y1;
  // Until I reach end of file
  while (!file.fail() && !file.eof()){
    std::string s;
    getline(file,s); // read the the whole line
    if(s.empty()) continue; // empty line
    //std::string::size_type posc=s.find('#'); // find #
    auto posc=s.find('#'); // find #
    if (posc==0) continue;// a comment line
    //std::string::size_type posd=s.find(':'); // find :
    auto posd=s.find(':');
    if(posd==std::string::npos)continue; // not found :
    std::string s2(s.substr(posd+1,posc)); // estract substring
    if(s2.empty())continue;
    std::istringstream tmp(s2); // build an input sstream.
    tmp>>x1>>y1; // read from the input stream
    if(!tmp.fail()){
      x.push_back(x1);
      y.push_back(y1);
    }
  }
  std::cout<<"I've read "<<x.size()<<" numbers from file datafile.dat:"<<std::endl;
  for (unsigned int i=0; i<x.size();++i) std::cout<<x[i]<<" "<<y[i]<<std::endl;
}
