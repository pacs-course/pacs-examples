#include <iostream>
#include <limits>
/*!
 \file scientific_precision.hpp 
 Set scientific precision to a stream
 
 In the form of a function and stream manipulator
*/
namespace PACS_Utilities
{
  /*!
   * A function that modifies a stream to set scientific precision and 
   * maximum number of digits for a floating point type
   * \tpar T a floating point type
   * \par out An output stream
   */
  template<typename T>
  void setMaximalPrecision(std::ostream & out)
  {
    auto constexpr maxprec=std::numeric_limits<T>::digits10;
    out.setf(std::ios_base::scientific, std::ios_base::floatfield);
    out.precision(maxprec+1);
  }
  //! A stream manipulator that set scientific and max precision for float
  std::ostream & maxPrecFloat(std::ostream & out)
  {
    setMaximalPrecision<float>(out);
    return out;
  }
  //! A stream manipulator that set scientific and max precision for double
  std::ostream & maxPrecDouble(std::ostream & out)
  {
    setMaximalPrecision<double>(out);
    return out;
  }
  //! A stream manipulator that set scientific and max precision for long double
  std::ostream & maxPrecLong(std::ostream & out)
  {
    setMaximalPrecision<long double>(out);
    return out;
  }
}
