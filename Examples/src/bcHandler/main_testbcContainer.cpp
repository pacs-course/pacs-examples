#include "bcContainer.hpp"
#include "string_utility.hpp"
#include <fstream>
#include <iostream>
#include <string>

#include "bcBcMuParserInterface.hpp"

//! @file An example of use of a container for boundary conditions
//! It reads the definition of the bc from a file calle "bc.txt" with the
//! following simple format Line 1: Number of boundary conditions then for each
//! bc: The description For example: Top boundary The bc type as a string. For
//! example Dirichlet The function: either zero or one or a string containing an
//! expresssion, for example: t +cos(x[0])*sin(x[1]) The number of entities
//! associated to the bc The list of entities (also on more than one line)
//!
int
main()
{
  using namespace apsc::FEM;
  std::ifstream file("bc.txt");
  if(file.bad())
    {
      std::cerr << "ERROR while opening file";
      return 2;
    }
  // read number of conditions
  int numBC;
  file >> numBC;
  std::cout << "The file contains data for " << numBC
            << " boundary conditions\n";
  Utility::cleanStream(file);
  apsc::FEM::BCContainer bCs;
  bCs.reserve(numBC);
  for(int i = 0; i < numBC; ++i)
    {
      apsc::FEM::BCBase bc;
      std::string       line;
      // read the description
      std::getline(file, line);
      bc.set_description(line);
      std::cout << "Reading BC with description " << line << std::endl;
      // read the type (beware of spaces, I use trim just in case)
      std::getline(file, line);
      line = Utility::trim(line);
      // Find the enum that corresponds to the string
      auto bctype_p = apsc::FEM::stringToBCType.find(line);

      if(bctype_p == apsc::FEM::stringToBCType.end())
        {
          std::cerr << "ERROR: Bc Type " << line << " not found\n";
          return 1;
        }
      bc.set_type(bctype_p->second);
      // now the function to be passed as a muparserx object wrapped in
      // astd::function
      std::getline(file, line);
      line = Utility::trim(line);
      std::cout << "Function: " << line << std::endl;
      // Treas special cases where I use directly a predefined function
      if(line == "zero")
        {
          bc.set_fun(apsc::FEM::zerofun);
        }
      else if(line == "one")
        {
          bc.set_fun(apsc::FEM::onefun);
        }
      else
        {
          // It's a muparserX expression
          bc.set_fun(apsc::FEM::bcMuParserInterface{line});
        }
      // The antites associated
      // first the number
      // the the list
      int                        numEntities;
      std::vector<apsc::FEM::Id> Ids;
      Ids.reserve(numEntities);
      file >> numEntities;
      for(int e = 0; e < numEntities; ++e)
        {
          Id entity;
          file >> entity;
          Ids.push_back(entity);
        };
      Utility::cleanStream(file);
      bc.set_entities(Ids);
      bCs.push_back(bc);
    }

  std::cout << "-------------List --------------\n";
  for(auto const &bc : bCs)
    {
      bc.showMe(std::cout);
      // testing the function
      // just to try if it works
      auto res = bc.apply(3., std::vector<double>{0., 0., 0.});
      std::cout << res << std::endl;
    }
}
