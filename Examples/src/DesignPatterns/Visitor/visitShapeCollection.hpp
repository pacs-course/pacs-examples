#ifndef HH_VISITSHAPECOLLECTION_HH
#define HH_VISITSHAPECOLLECTION_HH
#include "ShapeVisitor.hpp"
#include "geo.hpp"
/*! A function that visits a collection of shapes stored as
pointer in a container
  The container can be any container
*/
template <typename C>
void
visitShapeCollection(C &collection, Geometry2::ShapeVisitor &visitor)
{
  for(auto &i : collection)
    i->accept(visitor);
}
#endif
