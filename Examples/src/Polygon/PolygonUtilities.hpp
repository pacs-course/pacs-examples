#ifndef HH_POLYGONUTILITIES_HH
#define HH_POLYGONUTILITIES_HH
#include <string>
#include <memory>
#include "Polygon.hpp"
namespace Geometry
{
  //! A factory of Polygons
  /*!
    I am using perfect forwrding, so that make_unique gets the
    arguments with the same category as the ones of the function.
    If you think it is unnecessaryly complicated, you may use
    the simpler (but less efficint w.r.t. move semantic) version
    \code
    template<typename... Args>
  std::unique_ptr<AbstractPolygon> createPolygon(std::string name,const Args&... args)
     ...
      return std::make_unique<Polygon>(args...);
     ... //etc.
    \endcode
   */
  template<typename... Args>
  std::unique_ptr<AbstractPolygon> createPolygon(std::string const & name, Args&&... args)
  {
    if(name=="Polygon")
      return std::make_unique<Polygon>(std::forward<Args>(args)...);
    if(name=="Square")
      return std::make_unique<Square>(std::forward<Args>(args)...);
    if(name=="Triangle")
      return std::make_unique<Triangle>(std::forward<Args>(args)...);
    return std::unique_ptr<AbstractPolygon>();
  }
  
}

#endif
