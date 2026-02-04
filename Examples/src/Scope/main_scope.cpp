#include <iostream>
#include <vector>
/*! A simple exercise on the concept of scope.

Are you able to guess what will be printed on the terminal?
 */

// Global variables (in the global scope)
// Try to limit the use of global variable to the minumum possible!
// They can be source of name clashes
// Use namespaces to reduce the risk of name clashes
int g = 10;  // Global variable
int f = 30; // Global variable
// Namespace variables
namespace pippo
{
  int g = 40; // it hides ::g
  namespace pluto // nested namespace
  {
    int e = 30;
    int g = 15; // hides ::g and pippo::g;
  } // namespace pluto
} // namespace pippo
int
main()
{
  using namespace std; // std names injected in the current scope
  cout << "Here f is the global one " << f << endl;
  int f = 55; // Hides ::f (the global f) in this scope
  int g = -9999;    // Hides ::g (the global g) in this scope
  // you can still access the global variables using the scope resolution operator ::
  cout << "Local f=" << f << ", Global f=" << ::f << endl;
  cout << "Local g=" << g << ", Global g=" << ::g << endl;
  // also pippo::g and pippo::pluto::g are still accessible with their qulified
  // name, being namespace variables.
  cout << "pippo g " << pippo::g << ", pluto g=" << pippo::pluto::g << endl;
  { // A block scope
    double f =9.9; // overrides f in the outer scope, which is now inaccessible!
    // While the global f is still accessible using full qualified name
    // The g declared in  the enclosing scope is also accessible
    cout << "Local f=" << f << ", Global f=" << ::f << " g=" << g
        << endl;
    int g=5;// g in the outer scope is now inaccessible
    // but global g, pippo::g and pippo::pluto::g are still accessible with their qualified
    // name, being namespace variables.
    cout << "Local g"<< g << ", Global g=" << ::g << endl;
    cout << "pippo::g=" << pippo::g << " pluto::g=" << pippo::pluto::g << endl;
    double a =24.5; // variable in the block scope
    cout << "a=" << a << endl;
  }
// Here a is not accessible anymore
//  cout << "a=" << a << endl; // This would be an error!
{
  double a=12.3; // a new variable a in this new block scope
  cout << "a=" << a << endl;
}
  // for loops variables are local to the  loop scope!
  std::cout<<"For loop g values:"<<std::endl;
  for(int g = 1; g < 3; ++g)
    cout << g << " "; 
  cout << endl;
// Here g is the one declared in main! the g
  cout << "g is equal" << g << endl;
  // Suggestions: use long and significant names. Use namespace to reduce risk
  // of name clashes.
}
