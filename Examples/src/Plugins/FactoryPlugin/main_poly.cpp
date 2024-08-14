#include "Polygon.hpp"
#include "loadPolyFactory.hpp"
#include <dlfcn.h>
#include <iostream>

//
int
main()
{
  using namespace Geometry;
  using namespace std;
  // load the libraries (plugins)
  loadPolyFactory loadPlugins("plugins.txt");
  // Try the factory
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
          // Second entry of *where is the builder function!
          // I dereference it and I call the () operator: I get a Polygon
          auto thePoly = (where->second)();
          thePoly->showMe(cout);
        }
      else
        {
          cout << "ERROR: this polygon is not registered!" << endl;
        }
    }
  // I need to clear the factory because otherwise it may be destroyed
  // after loadPolyFactory is closed. But then the object in the factory
  // are not loaded anymore: segmentation fault.
  //  polyFactory.clear();
}
