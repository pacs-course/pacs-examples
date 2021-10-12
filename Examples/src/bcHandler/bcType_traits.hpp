/*
 * bcType_traits.hpp
 *
 *  Created on: Jan 13, 2021
 *      Author: forma
 */

#ifndef EXAMPLES_SRC_BCHANDLER_BCTYPE_TRAITS_HPP_
#define EXAMPLES_SRC_BCHANDLER_BCTYPE_TRAITS_HPP_
#include "string_utility.hpp"
#include <functional>
#include <map>
#include <string>
#include <vector>
//! This trait defines the main types for the bcHandler
//! For symplicity I do not create a structure but I keep the types just in a
//! namespace
namespace apsc::FEM
{
//! The type. None means no type yet assigned
enum BCType
{
  Dirichlet = 1,
  Neumann = 2,
  Robin = 3,
  Other = 4,
  None = 99
};
//! The type used to identify a boundary condition.
using BCName = std::string;
//! The type used to specify the space coordinates
using Coord = std::vector<double>;
//! The type of the function used to impose the bc
using BCFun = std::function<double(double const &t, Coord const &coord)>;

//! Type of the identifiers holding the index of the objects where the bc is
//! imposed
using Id = std::size_t;

/*!
 * A dictionary returning the BC corresponding to a name
 * Useful if you read the boundary condition name from a file and you want the
 * corresponding enumerator. To simplify things the comparison ignores case:
 * "Dirichlet" is equal to "DIrichlet" of "dirichlet" I am using an inline
 * variable (alternative: use extern, but then I need a source file to define
 * it!)
 */
inline const std::map<BCName, BCType, Utility::compareNoCase> stringToBCType{
  {"Dirichlet", Dirichlet},
  {"Neumann", Neumann},
  {"Robin", Robin},
  {"Other", Other},
  {"None", None}};

//! The zero function, as global variable
inline BCFun zerofun{[](double const, Coord const &) { return 0.0; }};

//! The one function, as global variable
inline BCFun onefun{[](double const, Coord const &) { return 1.0; }};

inline BCFun
make_constant_fun(double const &x)
{
  return [x](double const t, Coord const &) { return x; };
}

} // namespace apsc::FEM

#endif /* EXAMPLES_SRC_BCHANDLER_BCTYPE_TRAITS_HPP_ */
