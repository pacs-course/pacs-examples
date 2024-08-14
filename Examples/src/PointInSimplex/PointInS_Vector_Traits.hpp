#ifndef HHH_PointInS_Vector_Traits__HH
#define HHH_PointInS_Vector_Traits__HH
#ifdef HAVE_EIGEN
#include <Eigen/Core>
#else
#include <array>
#include <cmath>
#endif
namespace apsc{
#ifdef HAVE_EIGEN
    struct PointInS_Traits{
        using Point3D=Eigen::Vector3d;
        using Vector3D=Eigen::Vector3d;
        using Point2D=Eigen::Vector2d;
        using Vector2D=Eigen::Vector2d;
        using Vector4D=Eigen::Vector4d;
        using Tetrahedron=Eigen::Matrix<double,4,3>;
        using Triangle=Eigen::Matrix<double,3,2>;
        using Triangle3D=Eigen::Matrix<double,3,3>;
    };
/*!
 * @tparam Derived The type of the actual Eigen sparse matrix (or vector)
 * @param a a Eigen Vector
 * @param b a Eigen vector
 * @return the dot product
 */

template <class Derived1, class Derived2>
decltype(auto)
dot(const Eigen::MatrixBase<Derived1> &a, const Eigen::MatrixBase<Derived2> &b)
{
  return a.dot(b);
}
/*!
 * @tparam Derived The type of the actual Eigen sparse matrix (or vector)
 * @param a a Eigen matrix
 * @return the 2-norm
 */
template <class Derived>
decltype(auto)
norm(const Eigen::MatrixBase<Derived> &a)
{
  return a.norm();
}
/*!
 * @tparam Derived The type of the actual Eigen sparse matrix (or vector)
 * @param a a Eigen matrix represending a simplex
 * @param i the index of the vertex
 * @return the ith vertex of the simplex
 */
template <class Derived>
decltype(auto)
extract(const Eigen::MatrixBase<Derived> &a, int i)
{
  return a.row(i).transpose();
}
 #else
    struct PointInS_Traits{
        using Point3D=std::array<double,3>;
        using Vector3D=std::array<double,3>;
        using Point2D=std::array<double,2>;
        using Vector2D=std::array<double,2>;
        using Vector4D=std::array<double,4>;
        using Tetrahedron=std::array<Point3D,4>;
        using Triangle=std::array<Point2D,3>;
        using Triangle3D=std::array<Point3D,3>;
    };

template <std::size_t N>
auto
dot(const std::array<double,N> &a, const std::array<double, N> &b)
{
  auto result=0.0;
  for (auto i=0u;i<N;++i)
  {
    result+=a[i]*b[i];
  }
    return result;
}

template <std::size_t N>
auto
norm(const std::array<double,N> &a)
{
    return std::sqrt(dot(a,a));
}
template <class T, std::size_t N>
decltype(auto)
extract(const std::array<T,N> &a, int i)
{
  return a[i];
}
template <std::size_t N>
auto
operator + (const std::array<double,N> &a, const std::array<double,N> &b)
{
    std::array<double,N> result;
    for (auto i=0;i<N;++i)
        result[i]=a[i]+b[i];
    return result;
}
template <std::size_t N>
auto
operator -  (const std::array<double,N> &a, const std::array<double,N> &b)
{
    std::array<double,N> result;
    for (auto i=0u;i<N;++i)
        result[i]=a[i]-b[i];
    return result;
}
template <std::size_t N>
auto
operator -  (const std::array<double,N> &a)
{
    std::array<double,N> result;
    for (auto i=0u;i<N;++i)
        result[i]=-a[i];
    return result;
}
#endif
inline PointInS_Traits::Vector3D cross(const PointInS_Traits::Vector3D &a, const PointInS_Traits::Vector3D &b)
{
    PointInS_Traits::Vector3D result;
    result[0]=a[1]*b[2]-a[2]*b[1];
    result[1]=a[2]*b[0]-a[0]*b[2];
    result[2]=a[0]*b[1]-a[1]*b[0];
    return result;
}

inline auto cross(const PointInS_Traits::Vector2D &a, const PointInS_Traits::Vector2D &b)
{
    return a[0]*b[1]-a[1]*b[0];
}

}
#endif
