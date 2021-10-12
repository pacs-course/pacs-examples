#include "rational.hpp"
#include <fstream>
#include <vector>
/*Example of use of extendedAssert*/

//===============================================
int
main()
{
  using namespace std;
  vector<Rational> vRational;
  ifstream         myfile;
  // Apri file con in dati
  myfile.open("Rational.dat");
  int numer, denom;
  // Leggi i dati
  while(true)
    {
      myfile >> numer >> denom;
      if(!myfile.fail() && !myfile.eof())
        vRational.push_back(Rational(numer, denom));
      else
        break;
    }
  myfile.close();
  std::cout << "I have read " << vRational.size() << " Rationals :" << endl;
  for(vector<Rational>::iterator i = vRational.begin(); i != vRational.end();
      ++i)
    cout << *i << endl;
}
