#ifndef HILBERT_HPP
#define HILBERT_HPP 1

#include "linearAlgebra.hpp"

namespace LinearAlgebra {
  /*!
    \class Hilbert
    \brief A class to build a n times n Hilbert matrix
  */
  class Hilbert : public Matrix {
  public:
    /**
       @name Constructors
    */
    //@{
    Hilbert(int n) : Matrix(n, n) {
      for(int i = 0; i < n; i++)
	for(int j = 0; j < n; j++)
	  this->operator()(i,j) = 1 / ( (double)i + (double)j + 1 );
    }
    //@}
  };
}

#endif
