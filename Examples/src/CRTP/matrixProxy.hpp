#ifndef HH_MATRIXPROXY_HH
#define HH_MATRIXPROXY_HH
// A much more complex example of CRTP
// Suppose want to create different matrix classes, each of them
// providing the operator ()(i,j) di address the element in
// row i and column j. This is fine, but I would like also to be
// able to do the same with m[i][j] syntax, like in a "C-style" matrix
// defined as double **. Moreover the matrix may stora any type of
// elements, nit just double.
// This is a nice trik (from Discovering Modern C++).

// A proxy that stores the information to extract the row.
template <typename Matrix, typename Result>
class bracket_proxy
{
public:
  //! The constructor takes the matrix and the row
  bracket_proxy(Matrix & A, std::size_t r):A(A), r(r){};
  //! Returns the element giving the column
  Result& operator[] (std::size_t c){return A(r,c);}
  Result  operator[] (std::size_t c)const {return A(r,c);}
private:
  // A reference to the matrix!
  Matrix & A;
  //! the row
  std::size_t r;
};

//! The prototype of a matrix with [][] operator
template<typename Matrix, typename Result>
class crtp_matrix
{
  using const_proxy=bracket_proxy<const Matrix,const Result>;
  using proxy=bracket_proxy<Matrix,Result>;
public:
  //! Returns a proxy to the row
  proxy operator[](std::size_t r)
  {
    // I use brace initialization...
    return {static_cast<Matrix &>(*this),r};
  }
  const_proxy operator[](std::size_t r) const
  {
    // I use brace initialization...
    return {static_cast<const Matrix &>(*this),r};
  }
};
#endif
