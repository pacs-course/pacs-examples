/**
   \file givens.hpp
   \author Daniele A. Di Pietro <dipietro@unibg.it>
   \date 8-18-2005
*/

#ifndef GIVENS_HPP
#define GIVENS_HPP 1

#include "linearAlgebra.hpp"

namespace LinearAlgebra {
  /*!
    \class Givens
    \brief Apply Givens transformations to a matrix
    \author Daniele A. Di Pietro <dipietro@unibg.it>
  */

  class Givens {
  public:
    /** @name Constructors
     */
    //@{
    /*! Constructor taking the following parameters:
      @param m the first index
      @param l the second index 
      @param theta the rotation angle
    */
    Givens(int l, int m, double theta);
    /*! Constructor taking the following parameters:
      \param l the first index
      \param m the second index
      \param c the cosine of the rotation angle 
      \param s the sine of the rotation angle
    */
    Givens(int l, int m, double c, double s);
    //@}
    /** @name Accessors
     */
    //@{
    //! The cosine of the rotation angle
    inline double c() const { return M_c; }
    //! The sine of the rotation angle
    inline double s() const { return M_s; }
    //! The rotation angle
    inline double theta() const { return M_theta; }
    //! The first index
    inline int l() const { return M_l; }
    //! The second index
    inline int m() const { return M_m; }
    //@}
    /** @name Members
     */
    //@{
    //! Apply Givens transformation
    void apply(Matrix& /* A */);
    //@}
  private:
    //! First index
    int M_l;
    //! Second index
    int M_m;
    //! Rotation angle
    double M_theta;
    //! Cosine of the rotation angle
    double M_c;
    //! Sine of the rotation angle
    double M_s;
  };
}

#endif
