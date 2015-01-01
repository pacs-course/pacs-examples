#ifndef __BIFORC_HPP__
#define __BIFORC_HPP__
#include <memory>
#include "geo.hpp"

namespace Darcy{
  using namespace Geometry;
  typedef Eigen::Matrix<double,3,2> Matrix32;
  typedef Eigen::Matrix2d Matrix2d;
  typedef Eigen::Matrix<double,2,3> Matrix23;
  typedef Eigen::Matrix3d Matrix3d;
  typedef Eigen::Vector3d Vector3d;
  //! Computed the matrices needed for the building of the trasmissibility;
  class Biforc{
  public:
    //! Default constructor
    Biforc();
    //! Passing the permeability tensor
    Biforc(Matrix2d K);
    //! Passing a scalar permeability
    explicit Biforc(double K);
    //! Give the triangle.
    void setTriangle(Triangle const & T){triangle_=T;}
    //! Change the permeability.
    void setK(Matrix2d K){K_=K;}
    //! Get permeability.
    Matrix2d K()const{ return K_;}
    //! Compute matrix N having unscaled normals as rows.
    /*! 
      The internal triangle must have been set before calling this method.
    */
    void computeN();
    //! Get matrix N;
    Matrix32 N()const {return N_;}
    //! compute matrix C.
    /*!
      The matrix containing as rows the vector from triangle baricenter
      to triangle edge baricenter.
    */
    void computeC();
    //! get matrix C;
    Matrix32 C(){return C_;}
    //! compute matric Qc
    /*!
      The columns form an orthonormal basis for the transpose of C.
      It computes also C
    */
    void computeQc();
    //! get Qc.
    Matrix32 Qc()const {return Qc_;}
    //! Compute Projection matrix on the null space of Qc.
    /*
      Empty method since for a triangle PC is given
    */
    void computePc();
    //! Get Pc. I computes also Qc and C.
    Matrix3d Pc()const {return Pc_;}
    //! Computes the Trasmissibility matrix
    /*!
      It computes also C, N e Qc
      It uses the formula
      \f$ T= N K N^T + t P_c \operatorname{diag}(N K N^T) P_c
    */
    void computeT(double t=6.0);
    //! Computes the Trasmissibility matrix
    /*!
      It computes also C, N e Qc
      It uses the formula
      \f$ T= N K N^T + t \operatorname{trace}(NKN^T)P_c
    */
    void computeTsimple(double t=6.0);
    Matrix3d T()const {return T_;}
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW 
  private:
    Matrix2d K_;
    Triangle triangle_;
    Matrix32 N_;
    Matrix32 C_;
    Matrix32 Qc_;
    Matrix3d Pc_;
    Matrix3d T_;
  };
  //! Class that represents the end of a fracture
  struct FractureEnd
  {
    //! The end point of the fracture
    Point  endPoint;
    // The thickness of the fracture
    double thickness;
  };

  //! Class to represent the intersection
  class Intersection
  {
  public:
    //! Fractures must be set and given in clockwise order.
    Intersection(FractureEnd const & gamma0, FractureEnd const & gamma1, FractureEnd const & gamma2, Point const & intersectionPoint);
    Triangle const & computeIntersectionTriangle();
    Triangle const & intersectionTriangle()const {return intersectionTriangle_;}
  private:
    FractureEnd fractures[3];
    Point intersection_;
    Eigen::Vector2d tangents[3];
    Eigen::Vector2d normals[3];
    Triangle intersectionTriangle_;
  };

  //! Returns the coefficients that links the pressure to edge pressure
  /*!
    It computes the coefficients that links the pressure internal at the 
    intersection with that at the boundary (fracture end pressures).

    @param T transmissibility matrix.
    @return The vector the gives the coefficients.
   */
  Vector3d pressureCoeff(const Matrix3d & T);

}
#endif
