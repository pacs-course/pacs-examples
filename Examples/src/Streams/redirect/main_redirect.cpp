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
  std::string line{"Hello World"};
  int input=-1;
  do
    {
      std::cout<<" Do you want to print to terminal (0) or file (1)?\n";
      std::cin>>input;
    }
  while (not(input==1 || input==0));
  if (input == 1)
    {
      std::string filename;
      std::cout<<"Give me file name"<<std::endl;
     // Eliminate possible trailing carriage return or rubbish
      // Ignore all characters up and including carriage return
      // This is a good thing if you are using getline to read from std::cin
      // Because after the previous read cin may have some trailing characters
      // still in the buffer (and certainly it has a \n).
      // This command looks a bit weird but does the job.
      // You have an utilitu in Utility/string_utility.cpp if you do not want to
      // remember this command!
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      // get the whole line and put it into a string
      std::getline(std::cin,filename);
      // Using getline allows you to have file name with spaces in the name
      // SOMETHING YOU SHOULD AVOID!!!
      // If you replace getline by simply
      // std::cin>>filename;
      // it works but only with file with sensible names and with sensible
      // I mean without spaces (as a good file name should be).
      std::cout<<"Writing to file "<<filename<<std::endl;
      file.open(filename);
      // output is redirected to file
      std::cout.rdbuf(file.rdbuf());
    }
  std::cout<<line<<std::endl;
  // get back the normal buffer
  std::cout.rdbuf(cout_buff);
  std::cout<<"The End\n";
  // close the file
  if (input==1)file.close();
}

