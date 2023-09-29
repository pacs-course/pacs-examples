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
  auto    square_ptr = polyFactory(Shape::Square, origin, 3.0, 0.0);
  std::cout<<"A unique_pointer storing a Polygon:\n";
  square_ptr->showMe();
  /*
   * square_ptr is a unique pointer to AbstractPolygon: it means it has a unique ownership of the
   * resource (a Square in this case). If it exits out of scope or if I call reset()
   * the Square is destroyed.
   *
   * But I can pass a unique pointer in the constructor of a shared one!
   * By doing that the unique_ptr loses the ownership (it is emptied of the internal pointer), which is
   * passed to the shared pointer
   */

  // Here the unique_tr returned by PolyFactory is transferred to a shared_ptr.
  Point2D b=origin + Point2D{0.,1.};
  Point2D c=origin + Point2D{1.,0.};
  std::vector Vertices{origin,b,c};
  std::shared_ptr<AbstractPolygon> apoly(polyFactory(Shape::Triangle,Vertices) );
  std::shared_ptr<AbstractPolygon> bpoly(apoly);
  cout << "there are " << bpoly.use_count()
       << " shared pointers owning a polygon:" << endl;
  bpoly->showMe();
  apoly.reset(); // apoly is now empty
  cout << "after resetting apoly, there are " << bpoly.use_count()
       << " shared pointers owning a polygon" << endl;
  bpoly.reset(); // bpoly is now empty. Polygon is deleted
  cout << "after resetting bpoly there are " << bpoly.use_count()
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
