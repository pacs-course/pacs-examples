#include "string_utility.hpp"
#include <iostream>
#include <vector>
int main()
{
  using namespace std;
  string a("  I have spaces at my left and at my right ");
  string a_copy(a);
  cout<<"Original     :#"<<a<<"# string is between hashes"<<endl;
  cout<<"Trimmed  left:#"<<Utility::ltrim(a)<<"#"<<endl;
  // put back original: ltrim modifies the argument
  cout<<"Trimmed right:#"<<Utility::rtrim(a)<<"#"<<endl;
  cout<<"Trimmed fully:#"<<Utility::trim(a)<<"#"<<endl;
  cout<<"Upper Case   :"<<Utility::toupper(a)<<endl;
  cout<<"lower case   :"<<Utility::tolower(a)<<endl;
  Utility::GlobbedTextReader globbedText("test_string.cpp");
  std::stringstream & sstream = globbedText.globbedText();
  std::cout<<"the stream contains "<<sstream.str().size()<<" characters\n";
  while(!sstream.eof() && !sstream.bad())
    {
      std::string line;
      std::getline(sstream,line);
      std::cout<<line<<std::endl;
    }
  std::cout<<"the stream contains "<<sstream.str().size()<<" characters\n";
  sstream.clear();// clear flags in case of problems
  std::cout<<"Now the lines\n";
  // I have to reset the stream to read from the beginning
  sstream.seekg(0,std::ios::beg);
  auto lines = Utility::chop(sstream);
  sstream.str("");// clear buffer
  for (auto l : lines) std::cout<<l<<std::endl;
  std::cout<<" Now testing string distance\n";
  std::vector<std::string> v1{"Luca","John","cat","plain"};
  std::vector<std::string> v2{"Lucia","Mary","cut","plane"};
  std::cout<<"String1\tString2\tDistance\n";
  for (auto i=0u;i<v1.size();++i)
    {
      std::cout<<v1[i]<<"\t"<<v2[i]<<"\t"<<Utility::stringDistance(v1[i],v2[i])<<std::endl;
    }


}
