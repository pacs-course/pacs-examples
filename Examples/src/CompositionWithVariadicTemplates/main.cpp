#include "Bc.hpp"
#include "Id.hpp"
#include "Point.hpp"
#include "Extensions.hpp"
#include <iostream>
#pragma GCC diagnostic ignored "-Wunused-variable"

int
main()
{
  // to save typing the full qualified names
  using namespace apsc;
  using namespace apsc::FEM;
  using namespace apsc::Geometry;

  // A point
  using Point2dSimple = Point<double, 2>;
  //Another point
  using Point3dSimple = Point<double, 3>;
  // Composing a Point with an Id
  using Point2dWithId = Extension<Point2dSimple, Id>;
  // A point with ID and BC marker
  using Point2dWithIdAndBc = Extension<Point2dSimple, Id, Bc>;

  Point2dSimple     a{{7.,8.}}; // giving the coordinates
  Point2dWithId      b; //Default constructed
  Point2dWithIdAndBc c;
  Point3dSimple     e;

  a[0] = 1;
  a[1] = 5;
  b.setId(9);
  c.setBcType(BcType::Dirichlet);
  auto res = dot(a, b);
  /// auto res2=dot(a,e); ERROR dimension differs!!!
  Id ident{5};
  // I am passing the point and the constructor for the Id!
  std::array<double, 2> praw = {0., 1.5};
  Point2dSimple         psi{praw};
  // I am passing the point and the constructor for the Id!
  Point2dWithId          pwi{praw, ident};
  pwi.setId(4);
  std::cout << pwi.getId() << std::endl;
  Point2dWithIdAndBc z{praw,10,BcType::Neumann};
  std::cout << z.getId() << std::endl;
  //Point2dWithId K({0., 2.}, 3); wont work
  // since a Point does not have a constructor with an std::initializer_list
  Point2dWithId K(std::array<double, 2>{0., 2.}, 3);
  std::cout << K.getId() << std::endl;
}
