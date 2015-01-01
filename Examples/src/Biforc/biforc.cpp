#include "biforc.hpp"
namespace Darcy
{
  Biforc::Biforc():Pc_(Matrix3d::Constant(1./3.0))

  {
  }

  Biforc::Biforc(double K):Pc_(Matrix3d::Constant(1./3.0))
  {
    K_(0,0)=K;
    K_(0,1)=K_(1,0)=0.0;
    K_(1,1)=K;
  }

  Biforc::Biforc(Matrix2d K):K_(K),Pc_(Matrix3d::Constant(1./3.0)){};

  void Biforc::computeN()
  {
    N_.row(0)=triangle_.unscaledNormal(0).transpose();
    N_.row(1)=triangle_.unscaledNormal(1).transpose();
    N_.row(2)=triangle_.unscaledNormal(2).transpose();
  }

  void Biforc::computeC()
  {
    C_.row(0) = triangle_.c(0).transpose();
    C_.row(1) = triangle_.c(1).transpose();
    C_.row(2) = triangle_.c(2).transpose();
  }

  void Biforc::computeQc()
  {
    this->computeC();
    Qc_.col(0)  = C_.col(0).normalized();
    Vector3d v0 = Qc_.col(0);
    Vector3d v1 = C_.col(1) - ( v0.dot(C_.col(1)) )*v0;
    Qc_.col(1)  = v1.normalized();
  }

  void Biforc::computePc()
  {
    /*
    this->computeQc();
    Pc_= Matrix3d::Identity() - Qc_*Qc_.transpose();
    */
    // For a triangle we know how to do it!
  }

  void Biforc::computeT(double t)
  {
    //this->computePc();
    this->computeQc();
    this->computeN();
    double area = triangle_.measure();
    Matrix3d Nkn = N_*K_*N_.transpose();
    Eigen::DiagonalMatrix<double,3,3> Nd (Nkn.diagonal());
    Matrix3d tmp = Pc_ *Nd *Pc_;
    T_=(1./area)*( Nkn + t*tmp );
  }

  void Biforc::computeTsimple(double t)
  {
    //this->computePc();
    this->computeQc();
    this->computeN();
    double area = triangle_.measure();
    Matrix3d Nkn = N_*K_*N_.transpose();
    double Nd=Nkn.trace();
    Matrix3d tmp = Nd *Pc_;
    T_=(1./area)*( Nkn + t*tmp );
  }
   
  Intersection::Intersection(FractureEnd const & gamma0, FractureEnd const & gamma1, FractureEnd const & gamma2, Point const & intersectionPoint):
    fractures{gamma0,gamma1,gamma2}
  {
    Eigen::Vector2d tmp;
    for (unsigned int i=0;i<3;++i)
      {
	// get tangent at the end
	tmp(0) = intersectionPoint.x()-fractures[i].endPoint.x();
	tmp(1) = intersectionPoint.y()-fractures[i].endPoint.y();
	tmp.normalize();
	tangents[i]   = tmp;
	normals[i](0) = -tmp(1);
	normals[i](1) =  tmp(0);
      }
  }
  
  Triangle const &
  Intersection::computeIntersectionTriangle()
  {
    Point point;
    const double tol=1.e-5;
    double s(0.);
    for (unsigned int i=0; i<3;++i)
      {
	unsigned int j = (i +1 ) % 3;
	double ninj = normals[i].dot(normals[j] );
	double nitj = normals[i].dot(tangents[j]);
	if(std::fabs(nitj)<tol)
	  {
	    point=intersection_+Point(normals[i](0),normals[i](1))*
	      (0.5*(fractures[j].thickness+fractures[i].thickness));
	  }
	else
	  {
	    // parametric coordinate
	    s    = 0.5*(
			   fractures[j].thickness*ninj +
			   fractures[i].thickness
			   )/nitj;
	
	    // the ith point is Pji in the note
	    point = intersection_+ 
	      (Point(tangents[j](0),tangents[j](1))*s)- 
	      (Point(normals[j](0),normals[j](1))*(0.5*fractures[j].thickness));
	  }
	intersectionTriangle_.setPoint(i,point);
	   }
	return intersectionTriangle_;
  }
  Vector3d pressureCoeff(const Matrix3d &T)
  {
    double factor = (1.0/T.sum());
    Vector3d tmp;
    tmp(0)=factor*(T.col(0).sum());
    tmp(1)=factor*(T.col(1).sum());
    tmp(2)=factor*(T.col(2).sum());
    return tmp;
  }

}// end namespace Darcy
