
#ifndef HAVE_ABSTRACT_H
#define HAVE_ABSTRACT_H

class
abstract
{
public :
  virtual double eval (double) = 0;
  virtual ~abstract () { std::cout << "~abstract ()" << std::endl; };
};

#endif
