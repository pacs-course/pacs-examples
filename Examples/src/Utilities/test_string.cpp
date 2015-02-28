#include "string_utility.hpp"
#include <iostream>
int main()
{
  using namespace std;
  string a("  I have spaces at my left and at my right ");
  string a_copy(a);
  cout<<"Original     :#"<<a<<"# string is between hashes"<<endl;
  cout<<"Trimmed  left:#"<<Utility::ltrim(a)<<"#"<<endl;
  // put back original: ltrim modifies the argument
  a=a_copy;
  cout<<"Trimmed right:#"<<Utility::rtrim(a)<<"#"<<endl;
  a=a_copy;
  cout<<"Trimmed fully:#"<<Utility::trim(a)<<"#"<<endl;
  a=a_copy;
  cout<<"Upper Case   :"<<Utility::toupper(a)<<endl;
  a=a_copy;
  cout<<"lower case   :"<<Utility::tolower(a)<<endl;
}
