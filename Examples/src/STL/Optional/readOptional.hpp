/*
 * readOptional.hpp
 *
 *  Created on: Mar 29, 2020
 *      Author: forma
 */

#ifndef EXAMPLES_SRC_STL_OPTIONAL_READOPTIONAL_HPP_
#define EXAMPLES_SRC_STL_OPTIONAL_READOPTIONAL_HPP_
#include <optional>
#include <vector>
#include <iosfwd>
namespace apsc
{
//! protect names for this example
 namespace optionalExample
 {
   using Values=std::vector<std::optional<double> >;

   //! Read Values
   Values readValues(std::istream & in);
   //! Show the valid and not valid values
   std::ostream & operator<<(std::ostream & out, Values const & v);
 }
}



#endif /* EXAMPLES_SRC_STL_OPTIONAL_READOPTIONAL_HPP_ */
