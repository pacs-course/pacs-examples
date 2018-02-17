#ifndef HH_UMFPACK_EXAMPLE_HH
#define HH_UMFPACK_EXAMPLE_HH
#include <vector>
// Umfpack has C linkage!
extern "C"
{
#include "umfpack.h"
}
using Vector=std::vector<double>;
using Index=std::vector<int>;
//! Uses native umfpack to solve a sparse linear system
/*!  
 *   \param n size o the linear system
 *   \param Ap Indexes to column in a CSC format
 *   \param Ai Indexes to row  in a CSR format
 *   \param Ax Values in a CSR format
 *   \param b right hand side
 *   \returns solution
 *
 */
Vector solve_with_umfpack(int n,Index const & Ap, Index const & Ai, Vector const & Ax, Vector const & b);

#endif
