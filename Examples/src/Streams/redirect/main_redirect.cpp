#include <iostream>
#include <fstream>
#include <string>
#include <limits>
//! A simple example of stream redirection
int main()
{
  // save io buffer of cout. rdbuf returns a pointer
  auto cout_buff=std::cout.rdbuf();
  // a file stream
  std::ofstream file;
  std::string line("Hello World");
  int input=-1;
  do
    {
      std::cout<<" Do you want to print to terminal (0) or file (1)?\n";
      std::cin>>input;
    }
  while (input!=1 && input !=0);
  if (input == 1)
    {
      std::string filename;
      std::cout<<"Give me file name"<<std::endl;
      // Eliminate possible trailing carriage return or rubbish
      // Ignore all characters up and including carriage return
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::getline(std::cin,filename);
      std::cout<<"Writing to file "<<filename<<std::endl;
      file.open(filename);
      // output is redirected to file
      std::cout.rdbuf(file.rdbuf());
    }
  std::cout<<line<<std::endl;
  // get back the normal buffer
  std::cout.rdbuf(cout_buff);
  // close the file
  if (input==1)file.close();
}

