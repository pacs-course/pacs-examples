#include <iostream>
#include <numeric_limits>
/*!
 \file Set scientific precision to a stream
 
 The role of this example is just to remember how to do it!
*/

namespace PACS_Utilities
{
  template<typename T>
  void setMaximalPrecison(std::ostream & out)
  {
    auto maxprec=std::numeric_limits<T>::digits10;
    out.setf(ios::scientific);
    out.precision(maxprec+1);
  }
}
