/**
   \file matrix.hpp
   \author Daniele A. Di Pietro <dipietro@unibg.it>
   \date 8-18-2005
*/

#ifndef MATRIX_HPP
#define MATRIX_HPP 1

#include <iostream>
#include <cmath>
#include "vector.hpp"

namespace LinearAlgebra {
  /**
     \class Matrix
     \brief A class to handle full matrices
     \author Daniele A. Di Pietro <dipietro@unibg.it>
  */

  class Matrix {
  public:
    /** @name Constructors
     */
    //@{
    /*! Build the matrix from an array
      \param size1 first matrix dimension
      \param size2 second matrix dimension
      \param a the array to copy from
    */
    Matrix(int size1, int size2, double** a);
    /*! Build a matrix of a given size with all entries equal to a
      scalar
      \param size1 first matrix dimension
      \param size2 second matrix dimension
      \param s the constant value for all the entries
    */
    Matrix(int size1, int size2, double s);
    //! Copy constructor
    Matrix(const Matrix& m);
    //@}
    /** @name Destructors
     */
    //@{
    //! Destructor deleting the container
    ~Matrix();
    //@}
    /** @name Accessors
     */
    //@{
    //! Return the number of rows
    inline int size1() const { return M_size1; }
    //! Return the number of columns
    inline int size2() const { return M_size2; }
    //@}
    /** @name Assignments
     */
    //@{
    //! Assign
    Matrix& operator=(const Matrix&);
    //! Sum a matrix and assign the result
    Matrix& operator+=(const Matrix&);
    //! Subtract a matrix and assign the result
    Matrix& operator-=(const Matrix&);
    //@}
    /** @name Members
     */
    //@{
    //! Psi norm
    double psinorm() const;
    //! Frobenius norm
    double fronorm() const;
    //@}
    /** @name Accessors
     */
    //@{
    //! Double subscript access
    inline double* operator[](int i) const { return M_super[i]; }
    //! Subscript access
    inline double& operator()(int i, int j) { return M_super[i][j]; }
    //! Read-only subscript access
    inline const double& operator()(int i, int j) const { return M_super[i][j]; }
    //! Collect all diagonal entries in a vector
    friend Vector diag(const Matrix&);
    //@}
    /** @name Binary operations
     */
    //@{
    //! Sum a matrix
    friend Matrix operator+(const Matrix&, const Matrix&);
    //! Subtract a matrix
    friend Matrix operator-(const Matrix&, const Matrix&);
    //@}
    /** @name Output
     */
    //@{
    friend std::ostream& operator<<(std::ostream&, const Matrix&);
    //@}
    /** @name Matrix-vector multiplication
     */
    //@{
    Vector operator*(const Vector&) const;
    //@}
  protected:
    //! Number of rows
    int M_size1;
    //! Number of columns
    int M_size2;
    //! Container
    double** M_super;
  };

  /*!
    \class Hilbert
    \brief A class to build a n times n Hilbert matrix
  */
  struct Hilbert : public Matrix {
    /**
       @name Constructors
    */
    //@{
    Hilbert(int n) : Matrix(n, n, 0.) {
      for(int i = 0; i < M_size1; i++)
	for(int j = 0; j < M_size2; j++)
	  M_super[i][j] = 1 / ( (double)i + (double)j + 1 );
    }
    //@}
  };
  //! Print a matrix
  std::ostream& operator<<(std::ostream& /* ostr */, const Matrix& /* m */);
  //! Vector-matrix multiplication
  Vector operator*(const Vector& /* v */, const Matrix& /* A */);
  //! Extract the diagonal
  Vector diag(const Matrix& /* A */);
  //! Compute the Frobenius norm of a matrix
  double norm_fro(const Matrix& A);
  //! Compute the psi norm of a matrix
  double norm_psi(const Matrix& A);
}

#endif
