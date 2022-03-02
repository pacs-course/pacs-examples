#include <iostream>
#include <fstream>
#include <string>
#include "src/cg.hpp"
#include "src/solver_policies.hpp"

namespace UBLAS=boost::numeric::ublas;

int main() 
{
  // Using types defined in UblasTriPrecond (solver_policies.hpp)
  typedef UblasTriPrecond<double>::M M;
  typedef UblasTriPrecond<double>::V V;

  // Matrix A
  M A;

  // Open file
  std::ifstream in ( "matrix.mat" );

  // Check if file is readable
  if ( in.is_open() ) {
    // Support variables: read strings from file
    std::string line, i_str, j_str;

    // Search the marker
    /*
      NOTE: if not found, this is an infinite loop
    */
    do
      getline(in, line); 
    while (line.find("# SIZE") == std::string::npos);

    // Read (ni, nj) dimensions of A
    int ni, nj;
    in >> ni >> nj;
    A.resize(ni, nj);

    // Search the marker
    do
      getline(in, line); 
    while (line.find("# NZERO") == std::string::npos);

    // Read number of non zero elements
    int nzero;
    in >> nzero;

    // Parse and interpret each file line
    std::string::size_type openbr, comma, closebr;
    int i, j;
    double value;

    for(int k = 0; k < nzero; ++k) {
      in >> line;
      // Store char positions inside the line
      openbr = line.find('(',0); comma = line.find(',',0);
      closebr = line.find(')',0);
      // Substrings containing useful values
      i_str = line.substr(openbr+1,comma-1);
      j_str = line.substr(comma+1,closebr-1);
      // Convert strings to int
      i = atoi(i_str.c_str())-1;
      j = atoi(j_str.c_str())-1;
      // The A(i, j) value
      in >> value;
      A(i,j) = value;
    }
  }
  else
    std::cout << "\nData file problem" << std::endl;

  // Vector for right hand side
  /*
    Size of rhs = number of rows of A
    Initialized with ones
  */
  V rhs(A.size1());
  for (unsigned i = 0; i < rhs.size (); ++ i)
    rhs (i) = 1;

  // Vector for result
  /*
    Size of result = number of columns of A
    Initialized with zeros ( function clear() )
  */
  V result(A.size2());
  result.clear();

  // A very simple (identity) preconditioner
  IdentityPrecond<V> ident;

  // A matrix to initialize a triangular preconditioner
  /*
    precond_raw contains the diagonal of matrix A.
    You expect this preconditioner to have a good behaviour
    when applied to symmetric positive definite matrices
    (conditioning number will probably decrease)
   */
  M precond_raw ( zero_matrix<double> ( A.size1(), A.size2() ) );
  for (unsigned i = 0; i < A.size1 (); ++ i)
    {
      precond_raw(i,i) += A(i,i);
    }
  //  std::cout << precond_raw << std::endl;
  UblasTriPrecond<double> triang (precond_raw);

  // Problem parameters
  int max_iter(1000); double tol=1e-10; int status;

  // Solve with identity preconditioner
  status=cg(A,result,rhs,ident,max_iter,tol);
  std::cout << "\nIdentity Preconditioner"
	    << "\n\tstatus = " << status
	    << "\n\tnumber of iterations = " << max_iter
	    << "\n\terror = " << tol
    //	    << "\n\tresult = " << result
	    << std::endl;

  // Restore parameters
  result.clear(); tol=1e-10; max_iter=1000;

  // Solve with triangular preconditioner
  status=cg(A,result,rhs,triang,max_iter,tol);
  std::cout << "\nTriangular Preconditioner"
	    << "\n\tstatus = " << status
	    << "\n\tnumber of iterations = " << max_iter
	    << "\n\terror = " << tol
    //	    << "\n\tresult = " << result
	    << std::endl;
}
