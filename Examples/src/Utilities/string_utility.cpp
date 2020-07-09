#include "string_utility.hpp"
#include <fstream>
#include <exception>
#include <iostream>
namespace Utility
{
std::string toupper(std::string & s)
{
  // convert to upper case.`
  std::transform(s.begin(), 
		 s.end(),
		 s.begin(), 
		 [](std::string::const_reference c)
		 {
		   return std::toupper(c,std::locale());
		 } 
		 );
  return s;
}

std::string tolower(std::string & s)
{
  // convert to upper case.`
  std::transform(s.begin(), 
		 s.end(),
		 s.begin(), 
		 [](std::string::const_reference c)
		 {
		   return std::tolower(c,std::locale());
		 } 
		 );
  return s;
}

void GlobbedTextReader::read(std::string const & fileName)
{
  using namespace std;
  ifstream file (fileName.c_str(), ios::in|ios::binary|ios::ate);
  if (! file.is_open())
    {
      file.close();
      throw std::runtime_error("globbedTextHandler::read: cannot open file " + fileName);
    }
  streampos size;
  this->MySize = static_cast<std::size_t>(size);
  // get size looking for the end of the file
  size = file.tellg();
  // size buffer
  this->MyBuffer=std::make_unique<char[]>(size);
  // go back at the beginning
  file.seekg (0, ios::beg);
  // read all in the buffer
  file.read(this->buffer(), size);
  file.close();
  // Set the stringstream to use the read buffer and not the internal one
  this->MyGlobbedText.rdbuf()->pubsetbuf(MyBuffer.get(),size);
  // maybe is not needed
  this->setAtStart();
}

GlobbedTextReader::GlobbedTextReader(std::string const & fileName)
{
  this->read(fileName);
}



std::vector<std::string> chop (std::stringstream & sstream)
  {
    if(sstream.fail())
      throw std::runtime_error("chop: sstream is not in a valid state. Cannot read");

    std::vector<std::string> result;
    while(!sstream.eof() && !sstream.fail())
      {
        std::string line;
        std::getline(sstream,line);
        result.emplace_back(std::move(line));
      }
    return result;
}

}
