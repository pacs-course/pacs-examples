#include <iostream>
#include <vector>
/*! A simple exercise on the concept of scope.

Are you able to guess what will be printed on the terminal?
 */

// Global variables (in the global scope)
// Try to limit the use of global variable to the minumum (i.e. 0)
int g = 10;
int f = 30;
// Namespace variable
namespace pippo
{
  int g = 40; // it hides ::g
  namespace pluto
  {
    int e = 30;
    int g = 15; // hides ::g and pluto::g;
  } // namespace pluto
} // namespace pippo
int
main()
{
  using namespace std; // std names injected in the current scope
  int f = 55; // Hides ::f
  int g = -9999;    
  cout << "Local g=" << g << ", Global g=" << ::g << endl;
  cout << "pippo g " << pippo::g << ", pluto g=" << pippo::pluto::g << endl;
  { // A block scope
    double f =
        9.9; // overrides f in the outer scope, which is now inaccessible!
    // While the global f is still accessible using full qualified name
    // The g declared in  the enclosing scope is also accessible
    cout << "Local f=" << f << ", Global f=" << ::f << " g=" << g
        << endl;
    // also pippo::g and pippo::pluto::g ar still accessible with their qulified
    // name, being namespace variables.
    cout << "pippo::g=" << pippo::g << " pluto::g=" << pippo::pluto::g << endl;
  }
  cout << " main scope f=" << f << endl;
  // for loops variables are in a  loop scope!
  for(int g = 1; g < 3; ++g)
    cout << g << " "; 
  cout << endl;

  cout << "g is equal" << g << endl;
  // Suggestions: use long and significant names. Use namespace to reduce risk
  // of name clashes.
}
