#include "GetPot.hpp"
#include "readParameters.hpp"
#include <fstream>
parameters readParameters(std::string const & filename,bool verbose)
{
  // Parameter default constructor fills it with the defaults values
  parameters defaults;
  // checks if file exixts and is readable
  std::ifstream check(filename); 
  if(!check)
    {
      std::cerr<<"ERROR: Parameter file "<<filename<<" does not exist"<<std::endl;
      std::cerr<<"Reverting to default values."<<std::endl;
      if(verbose) std::cout<<defaults;
      check.close();
      return defaults;
    }
  else
    check.close();

  GetPot ifile(filename.c_str());
  parameters values;
  // Read parameters from getpot ddata base
  values.itermax=ifile("itermax",defaults.itermax);
  values.toler=ifile("toler",defaults.toler);
  values.L=ifile("L",defaults.L);
  values.a1=ifile("a1",defaults.a1);
  values.a2=ifile("a2",defaults.a2);
  values.To=ifile("To",defaults.To);
  values.Te=ifile("Te",defaults.Te);
  values.k=ifile("k",defaults.k);
  values.hc=ifile("hc",defaults.hc);
  values.M=ifile("M",defaults.M);
  if(verbose)
    {
      std::cout<<"PARAMETER VALUES IN GETPOT FILE"<<"\n";
      ifile.print();
      std::cout<<std::endl;
      std::cout<<"ACTUAL VALUES"<<"\n"<<values;
    }
  return values;
}
