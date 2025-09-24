#include "parameters.hpp"
#include <iostream>
// Overloads the stream insertion operator to print all parameter values in a
// readable format. Usage: std::cout << parameters_instance;
std::ostream &
operator<<(std::ostream &out, const parameters &p)
{
  out << "itermax= " << p.itermax << "\n";
  out << "toler= " << p.toler << "\n";
  out << "L= " << p.L << "\n";
  out << "a1= " << p.a1 << "\n";
  out << "a2= " << p.a2 << "\n";
  out << "To= " << p.To << "\n";
  out << "Te= " << p.Te << "\n";
  out << "k= " << p.k << "\n";
  out << "hc= " << p.hc << "\n";
  out << "M= " << p.M << "\n";
  out << "Solver type= " << p.solverType << "\n\n";
  return out;
}
