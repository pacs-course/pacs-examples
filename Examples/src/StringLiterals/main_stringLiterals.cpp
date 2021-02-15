#include <string>
#include <iostream>
int main()
{

  std::string interpreted="Hello\nWorld";
  std::string notInterpreted=R"!(Hello\nWorld)!";

  std::cout<<"interpreting \\n as newline:"<<std::endl;
  std::cout<<interpreted<<std::endl;
  std::cout<<"treating \\n verbatim:"<<std::endl;
  std::cout<<notInterpreted<<std::endl;

  // Now something surprising...

  std::string trunk = "Hello\0World";
  std::string noTrunk = R"!(Hello\0World)!";
  std::cout<<"interpreting end of string  \\0:"<<std::endl;
  std::cout<<trunk<<std::endl;
  std::cout<<"treating end of string  \\0 verbatim:"<<std::endl;
  std::cout<<noTrunk<<std::endl;
}
