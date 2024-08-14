#ifndef HH_POINTINSIMPLEX_HH
#define HH_POINTINSIMPLEX_HH
#include "PointInS_Vector_Traits.hpp"
#include <tuple>
namespace apsc
{
    /*!
        * @brief Check if a point is inside a tetrahedron
        * @param p the point
        * @param t the tetrahedron
        * @return a tuple with the first element being a boolean indicating if the point is inside the tetra
        * the other four elements are the baricentric coordinates associated to the corresponding vertexes.
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
    bool in = (l1 >= 0.0) && (l2 >= 0.0) && (l3 >= 0.0) && (l0 >= 0.0);
    return {in,{l0,l1, l2, l3}};
}
/*!
        * @brief Check if a point is inside a triangle
        * @param p the point
        * @param t the triangle
        * @return a tuple with the first element being a boolean indicating if the point is inside the triangle
        * the other three elements are the baricentric coordinates associated to the corresponding vertexes.
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
    bool in = (l1 >= 0.0) && (l2 >= 0.0) && (l0 >= 0.0);
    return {in,{l0, l1, l2}};
}

/*!
        * @brief Check if a point is inside a triangle immesed in a 3D space
        * @param p the 3D point
        * @param t the triangle
        * @param eps the tolerance used to check if the point is inside the triangle plane
        * @return a tuple with the first element being a boolean indicating if the point is inside the triangle
        * the other three elements are the baricentric coordinates associated to the corresponding vertexes.
        *
        * The code is more complicated than the 2D case since we need to compute the normal to the triangle
        * and the distance of the point from the plane containing the triangle.
        * 
        * @note The baricentric coordinates are computed using the formula
        * \f[
        * \lambda_i=\frac{V_i}{V}
        * \f]
        * where \f$V\f$ is the area of the triangle and \f$V_i\f$ is the area of the triangle
        * obtained by replacing the \f$i\f$-th vertex with the point \f$w\f$ obtained by projecting
        * \f$p\f$ on the triangle plane.
        * 
        * @note The projection of \f$p\f$ on the triangle plane is computed using the formula
        * \f[
        * w=p-\frac{\langle n,p-v_0\rangle}{\langle n,n\rangle}n
        * \f]
        * In fact, we do not need to compute it esplicitely, since the formula for the baricentric coordinates
        * eliminates the component of \f$w- v_0\f$ along the normal to the triangle.
    */
inline std::tuple<bool, PointInS_Traits::Vector3D>
 pointInTriangle3(const PointInS_Traits::Point3D &p, const PointInS_Traits::Triangle3D &t, double eps=1.e-3)
 {
        using Vector=PointInS_Traits::Vector3D; // just to make the code more readable
        Vector v01 = extract(t, 1) - extract(t, 0);
        Vector v02 = extract(t, 2) - extract(t, 0);
        Vector s   = p - extract(t, 0);
        Vector n = cross(v01,v02);
        double A = norm(n);
        auto z = dot(n,s);
        if (std::abs(z)>eps*A)
            return {false,{0.0,0.0,0.0}};
        A*=A; // square it   
        auto l1 = dot(cross(s, v02), n) / A;
        auto l2 = dot(cross(v01, s), n) / A;
        auto l0 = 1.0-l1-l2;
        bool in = (l1 >= 0.0) && (l2 >= 0.0) && (l0 >= 0.0);
        return {in,{l0,l1, l2}};
 }

}// end namesoace apsc
#endif