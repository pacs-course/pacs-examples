#include "Point.hpp"
#include "Id.hpp"
#include "Bc.hpp"
#include <iostream>
int main()
{
  using namespace Identifier;
  
  using Point2dSimple = Point<double,2>;
  using Point3dSimple = Point<double,3>;
  using Poin2dWithId  = Point<double,2,Id>;
  using Poin2dWithIdAndBc  = Point<double,2,Id,Bc>;

  Point2dSimple a;
  Poin2dWithId  b;
  Poin2dWithIdAndBc c;
  Point3dSimple e;

  a[0]=1;
  a[1]=5;
  b.setId(9);
  c.setBcType(BcType::Dirichlet);
  auto res=dot(a,b);
  /// auto res2=dot(a,e); ERROR dimension differs!!!
  Id ident(5);
  
  Poin2dWithId pwi(ident);
  std::cout<<pwi.getId()<<std::endl;
  Poin2dWithId z=pwi;
  std::cout<<z.getId()<<std::endl;
  Poin2dWithId K(3);
  std::cout<<K.getId()<<std::endl;
  
}

