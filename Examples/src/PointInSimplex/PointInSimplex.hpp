#ifndef HH_POINTINSIMPLEX_HH
#define HH_POINTINSIMPLEX_HH
#include "PointInS_Vector_Traits.hpp"
#include <tuple>
#include <iostream>
namespace apsc
{
    /*!
        * @brief Check if a point is inside a tetrahedron
        * @param p the point
        * @param t the tetrahedron
        * @return a std::tuple<bool, PointInS_Traits::Vector4D> where the first element is a boolean indicating if the point is inside the tetrahedron,
        * and the second element is a Vector4D containing the barycentric coordinates associated to the corresponding vertices.
        *
        * @note The baricentric coordinates are computed using the formula
        * \f[
        * \lambda_i=\frac{V_i}{V}
        * \f]
        * where \f$V\f$ is the volume of the tetrahedron and \f$V_i\f$ is the volume of the tetrahedron
        * obtained by replacing the \f$i\f$-th vertex with the point \f$p\f$.
 
    */ 
inline std::tuple<bool,PointInS_Traits::Vector4D>
 pointInTetrahedron(const PointInS_Traits::Point3D &p, const PointInS_Traits::Tetrahedron &t)
{
    using Vector=PointInS_Traits::Vector3D; // just to make the code more readable
    Vector v01 = extract(t, 1) - extract(t, 0);
    Vector v02 = extract(t, 2) - extract(t, 0);
    Vector v03 = extract(t, 3) - extract(t, 0);
    Vector s   = p - extract(t, 0);
    auto c23 = cross(v02, v03);
    auto V  = dot(v01, c23);
    auto l1 = dot(c23, s) / V;
    auto l2 = dot(cross(v03, v01), s) / V;
    auto l3 = dot(cross(v01, v02), s) / V;
    auto l0=1.0-l1-l2-l3;
    constexpr double tol = -1e-12;// small negative tolerance to account for round off errors
    bool in = (l1 >= tol) && (l2 >= tol) && (l3 >= tol) && (l0 >= tol);
    return {in,{l0,l1, l2, l3}};
}
/*!
        * @brief Check if a point is inside a triangle
        * @param p the point
        * @param t the triangle
        * @return a std::tuple<bool, PointInS_Traits::Vector3D> where the first element is a boolean indicating if the point is inside the triangle,
        * and the second element is a Vector3D containing the barycentric coordinates associated to the corresponding vertices.
        *
        * @note The baricentric coordinates are computed using the formula
        * \f[
        * \lambda_i=\frac{V_i}{V}
        * \f]
        * where \f$V\f$ is the area of the triangle and \f$V_i\f$ is the area of the triangle
        * obtained by replacing the \f$i\f$-th vertex with the point \f$p\f$.
 
    */

inline std::tuple<bool, PointInS_Traits::Vector3D>
 pointInTriangle(const PointInS_Traits::Point2D &p, const PointInS_Traits::Triangle &t)
{    
    using Vector=PointInS_Traits::Vector2D; // just to make the code more readable
    Vector v0=extract(t,0);
    Vector v01 = extract(t, 1) - v0;
    Vector v02 = extract(t, 2) - v0;
    Vector s   = p - v0;
    auto V  = cross(v01, v02);
    auto l2 = cross(v01, s) / V;
    auto l1 = cross(s, v02) / V;
	auto l0=1.0-l1-l2;
    constexpr double tol = -1e-12;// small negative tolerance to account for round off errors
    bool in = (l1 >= tol) && (l2 >= tol) && (l0 >= tol);
    return {in,{l0, l1, l2}};
}

/*!
	* @brief Returns a tuple with the following elements:
	* - bool: true if the point is inside the triangle (using barycentric coordinates)
	* - bool: true if the point is on the triangle plane (within tolerance eps)
	* - double: signed distance from the point to the triangle plane
	* - Vector3D: barycentric coordinates (for the projected point if not on the plane)
	*
	* @param p the 3D point
	* @param t the triangle in 3D
	* @param eps the tolerance used to check if the point is on the triangle plane (default: 1.e-3)
	* @returns The tuple
	* @note The barycentric coordinates are computed for the projection of p onto the triangle plane.
*/
inline std::tuple<bool, bool, double, PointInS_Traits::Vector3D>
 pointInTriangle3(const PointInS_Traits::Point3D &p, const PointInS_Traits::Triangle3D &t, double eps=1.e-3)
 {
        using Vector=PointInS_Traits::Vector3D; // just to make the code more readable
        Vector v01 = extract(t, 1) - extract(t, 0);
        Vector v02 = extract(t, 2) - extract(t, 0);
        Vector s   = p - extract(t, 0);
        Vector n = cross(v01,v02);
        double A = norm(n);
        auto z = dot(n,s)/A;
		bool Onplane=std::abs(z)<=eps;
        A*=A; // square it   
        auto l1 = dot(cross(s, v02), n) / A;
        auto l2 = dot(cross(v01, s), n) / A;
        auto l0 = 1.0-l1-l2;
		constexpr double tol = -1e-12; // small negative tolerance to account for round off errors
        bool in = (l1 >= tol) && (l2 >= tol) && (l0 >= tol);
        return {in,Onplane,z,{l0,l1, l2}};
 }

}// end namespace apsc
#endif