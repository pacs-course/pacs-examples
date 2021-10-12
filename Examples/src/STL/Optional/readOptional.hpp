/*
 * readOptional.hpp
 *
 *  Created on: Mar 29, 2020
 *      Author: forma
 */

#ifndef EXAMPLES_SRC_STL_OPTIONAL_READOPTIONAL_HPP_
#define EXAMPLES_SRC_STL_OPTIONAL_READOPTIONAL_HPP_
#include <iosfwd>
#include <optional>
#include <vector>
namespace apsc
{
//! protect names for this example
namespace optionalExample
{
  using Values = std::vector<std::optional<double> >;

  //! Read Values
  Values readValues(std::istream &in);
  //! Show the valid and not valid values
  std::ostream &operator<<(std::ostream &out, Values const &v);
} // namespace optionalExample
} // namespace apsc

#endif /* EXAMPLES_SRC_STL_OPTIONAL_READOPTIONAL_HPP_ */
