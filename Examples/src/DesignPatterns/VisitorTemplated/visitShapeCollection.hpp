#ifndef HH_VISITSHAPECOLLECTION_HH
#define HH_VISITSHAPECOLLECTION_HH
#include "geo.hpp"
#include "ShapeVisitor.hpp"
/*! A function that visits a collection of shapes stored as 
pointer in a container
  The contained can be any container
*/
template <typename C>
void visitShapeCollection(C & collection,
                          Geometry::ShapeVisitor & visitor)
{
  for (auto & i: collection) i->accept(visitor);
}
#endif
