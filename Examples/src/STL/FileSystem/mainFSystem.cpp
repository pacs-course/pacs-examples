/*
 * mainFSystem.cpp
 *
 *  Created on: Apr 4, 2020
 *      Author: forma
 */
#include "GetPot"
#include <filesystem>
#include <iostream>
#include <optional>
#include <string>
//! prints synopsis
void
printHelp()
{
  std::cout << "mainFSystem [-h] -f filename|dirname" << std::endl;
}
//! An example of the filesystem utilities of the standard library
int
main(int argc, char *argv[])
{
  GetPot gp(argc, argv);
  bool   help = gp.search(2, "-h", "--help");
  if(help)
    {
      printHelp();
      return 0;
    }
  std::string fileToExhamine = gp.follow("NULL", "-f");
  if(fileToExhamine == "NULL")
    {
      std::cout << " Error: file not valid\n";
      printHelp();
      return 1;
    }
  std::filesystem::path filePath{fileToExhamine}; // create a path

  if(std::filesystem::is_regular_file(filePath)) // is a regular file?
    {                                            // is path p a regular file?
      std::cout << filePath << " is a regular file of "
                << std::filesystem::file_size(filePath) << " bytes\n";
    }
  else if(std::filesystem::is_directory(filePath)) // is a directory?
    {
      std::cout << filePath << " is a directory containing:\n";
      auto dirIterator = std::filesystem::directory_iterator{filePath}; // to iterate the directory
      for(auto const &e : dirIterator)
        {
          std::cout << e.path() << std::endl;
        }
    }
  else if(std::filesystem::exists(filePath)) // does it exists?
    {
      std::cout << filePath << " is a special file:\n";
    }
  else
    {
      std::cout << filePath << " does not exists:\n";
    }
}
