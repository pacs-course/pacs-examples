#include <string>
#include <iostream>
int main()
{
  std::string interpreted="Hello\nWorld";
  std::string notInterpreted="Hello\nWorld"s;

  std::cout<<"interpreting \\n as newline:"<<std::endl;
  std::cout<<interpreted<<std::endl;
  std::cout<<"interpreting \\n verbatim:"<<std::endl;
  std::cout<<NotInterpreted<<std::endl;


}
