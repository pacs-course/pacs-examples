#include <cstring>
#include <iostream>

#include "abstract.h"
#include "concrete1.h"
#include "concrete2.h"

class
factory
{
private :
  abstract *myclass;
public :
  factory (char *which_class)
  {
    if (! strcmp (which_class, "class1"))
      myclass = new concrete1;
    else if (! strcmp (which_class, "class2"))
      myclass = new concrete2;
    else throw (std::out_of_range ("unexpected option"));
  };

  ~factory ()
  {
    delete myclass;
  };

  double eval (double x) {return myclass->eval (x); };
};

int main ()
{
  
  char opt[255] = "class1";
  factory f (opt);

  std::cout << "f.eval (1.0) = " << f.eval (1.0) << std::endl;

}
