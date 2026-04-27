#include "FactoryTraits.hpp"
#include "Polygon.hpp"
#include "loadPolyFactory.hpp"
#include <iostream>

/*!
 * @brief Demonstrates plugin-based registration of polygon builders.
 *
 * The program loads the shared libraries listed in `plugins.txt`. Their
 * constructor functions register builders into `polyFactory`. The user can
 * then request a polygon by name, and the corresponding object is created at
 * runtime through the registered builder.
 */
int
main()
{
  using namespace Geometry;
  using namespace std;
  // Load the plugin libraries listed in plugins.txt.
  loadPolyFactory loadPlugins("plugins.txt");
  // Query the shared factory populated by the plugins.
  while(true)
    {
      string answer;
      cout << "Quadrilateral, Triangle, Square or END?" << endl;
      cin >> answer;
      if(answer == "END")
        {
          break;
        }
      auto where = polyFactory.find(answer);
      if(where != polyFactory.end())
        {
          // The mapped value is the builder associated with the chosen key.
          auto thePoly = (where->second)();
          thePoly->showMe(cout);
        }
      else
        {
          cout << "ERROR: this polygon is not registered!" << endl;
        }
    }
  // In a larger program you should clear the factory before unloading the
  // libraries, otherwise the stored builders would refer to unloaded code.
  //  polyFactory.clear();
}
