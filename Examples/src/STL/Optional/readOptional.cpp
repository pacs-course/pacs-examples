#include "readOptional.hpp"
#include <iostream>
#include <vector>

apsc::optionalExample::Values
apsc::optionalExample::readValues(std::istream &in)
{
  // read number of entries
  int Nentries;
  in >> Nentries;
  // I use resize so that the default constructor of optional is called
  Values values;
  values.resize(Nentries);
  // read the rest. No check done on stream, so if the file does not
  // contains what it should, you have a runtime error
  while(!in.eof())
    {
      double v;
      int    i;
      in >> i >> v;
      values[i - 1] = v;
    }
  return values;
}

std::ostream &
apsc::optionalExample::operator<<(std::ostream &out, const Values &v)
{
  for(auto i = 0u; i < v.size(); ++i)
    {
      out << i + 1 << "\t";
      if(v[i])
        {
          out << *v[i];
        }
      else
        {
          out << "Unset";
        }
      out << std::endl;
    }
  return out;
}
/*
 * readOptional.cpp
 *
 *  Created on: Mar 29, 2020
 *      Author: forma
 */
