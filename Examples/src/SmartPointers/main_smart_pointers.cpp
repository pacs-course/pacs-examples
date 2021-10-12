#include "enable_shared.hpp"
#include "polyFactory.hpp"
#include <iostream>
//! An example of smart pointers.
int
main()
{
  using namespace Geometry;
  using namespace std;

  // Use the factory to create a polygon
  Point2D origin(1., 1.);
  auto    square_ptr = polyFactory<Shape::Square>(origin, 3.0, 0.0);
  square_ptr->showMe();

  std::shared_ptr<AbstractPolygon> apoly(
    polyFactory<Shape::Triangle>().release());
  std::shared_ptr<AbstractPolygon> bpoly(apoly);
  cout << "there are " << bpoly.use_count()
       << " shared pointers owning a polygon" << endl;
  bpoly->showMe();
  apoly.reset(); // apoly is now empty
  cout << "there are " << bpoly.use_count()
       << " shared pointers owning a polygon" << endl;
  bpoly.reset(); // bpoly is now empty. Polygon is deleted
  cout << "there are " << bpoly.use_count()
       << " shared pointers owning a polygon" << endl;

  std::cout << "Trying the enable_shared facility\n";
  auto tp1 = make_shared<Try>();
  tp1->set(5.0);
  cout << "I have " << tp1.use_count() << " pointers to Try objects\n";
  cout << "with value " << tp1->get() << std::endl;
  // The shared_from_this feature
  auto tp2 = tp1->shared_from_this();
  cout << "I have " << tp1.use_count() << " pointers to Try objects\n";
  cout << "with value " << tp1->get() << " and " << tp2->get() << std::endl;
}
