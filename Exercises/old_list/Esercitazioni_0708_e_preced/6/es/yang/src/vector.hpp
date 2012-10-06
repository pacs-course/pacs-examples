/**
   \file vector.hpp
   \author Daniele A. Di Pietro <dipietro@unibg.it>
   \date 8-18-2005
*/

#ifndef VECTOR_HPP
#define VECTOR_HPP 1

#include <iostream>
#include <cmath>

namespace LinearAlgebra {
  /*!
    \class Vector
    \brief A class to handle vectors
    \author Daniele A. Di Pietro <dipietro@unibg.it>
  */

  struct Vector {
    /** @name Constructors
     */
    //@{
    /*! Build the vector from an array
      \param n vector size
      \param a the array to copy from
    */
    Vector(int n, double* a);
    /*! Build a vector of given size with all entries equal to 
      \param n vector size
      \param a the array to copy from
    */
    Vector(int n = 0, double a = 0.);
    //! Copy constructor
    Vector(const Vector&);
    //@}
    /** @name Destructors
     */
    //@{
    //! Desctructor deleting the container
    ~Vector(){ delete[] M_super; }
    //@}
    /** @name Assignments
     */
    //@{
    //! Assign
    Vector& operator=(const Vector&);
    //! Sum a vector and assign the result
    Vector& operator+=(const Vector&);
    //! Subtract a vector and assign the result
    Vector& operator-=(const Vector&);
    //! Divide by a scalar and assign the result
    Vector& operator/=(double);
    //@}
    /** @name Accessors
     */
    //@{
    //! Return the size of the vector
    inline int size() const { return M_size; }
    //! Return the i-th entry
    inline double& operator[](int i ) { return M_super[i]; }
    //! Read-only access to the i-th entry
    inline const double& operator[](int i) const { return M_super[i]; }
    //@}
    /** @name Unary operations
     */
    //@{
    //! Unary +
    friend Vector operator+(const Vector&);
    //! Unary -
    friend Vector operator-(const Vector&);
    //@}
    /** @name Binary operations
     */
    //{
    //! Sum a vector of the same size
    friend Vector operator+(const Vector&, const Vector&);
    //! Subtract a vector of the same size
    friend Vector operator-(const Vector&, const Vector&);
    //! Scalar-vector multiplication
    friend Vector operator*(double, const Vector&);
    //! Vector-scalar multiplication
    friend Vector operator*(const Vector&, double);
    //! Vector-scalar division
    friend Vector operator/(const Vector&, double);
    //! Dot product
    friend double dot(const Vector&, const Vector&);
    //}

    /** @name Output
     */
    //@{
    //! Send the vector to an output stream
    friend std::ostream& operator<<(std::ostream&, const Vector&);
    //@}

  private:
    //! Size of the vector
    int M_size;
    //! Container
    double* M_super;
  };

  //! Compute the Euclidean norm of a vector
  double norm_2(const Vector& v);
  //! Compute the maximum norm of a vector
  double norm_inf(const Vector& v);
}

#endif
