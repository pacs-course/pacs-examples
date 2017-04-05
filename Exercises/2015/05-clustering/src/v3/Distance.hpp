#ifndef DISTANCE_HPP
#define DISTANCE_HPP

#include "Geo.hpp"

template <typename ObjectT>
struct EuclideanDistance {};

// specialization
template <>
struct EuclideanDistance<Point2D>
{
    typedef Point2D object_T;
    typedef std::vector<object_T> objectList_T;

    real operator()( object_T const & p1, object_T const & p2 )
    {
        return ( p1 - p2 ).norm();
    }

    object_T findCentroid( objectList_T const & objList );
};

Point2D EuclideanDistance<Point2D>::findCentroid( objectList_T const & objList )
{
    object_T centroid = object_T::Constant( 0. );
    for( size_t kObj = 0; kObj < objList.size(); kObj++ )
        centroid += objList[kObj];
    centroid /= objList.size();
    return centroid;
}

template <typename ObjectT>
struct AnisotropicDistance {};

// specialization
template <>
struct AnisotropicDistance<Point2D>
{
    typedef Point2D object_T;
    typedef std::vector<object_T> objectList_T;
    typedef Eigen::Matrix2d tensor_T;

    void setTensor( tensor_T const & tensor )
    {
        M_tensor = tensor;
    }

    real operator()( Point2D const & p1, Point2D const & p2 )
    {
        return ( p1 - p2 ).dot( M_tensor * ( p1 - p2 ) );
    }

    object_T findCentroid( objectList_T const & objList );

private:
     tensor_T M_tensor;
};

Point2D AnisotropicDistance<Point2D>::findCentroid( objectList_T const & objList )
{
    object_T centroid = object_T::Constant( 0. );
    for( size_t kObj = 0; kObj < objList.size(); kObj++ )
        centroid += M_tensor * objList[kObj];
    centroid = M_tensor.inverse() * centroid;
    centroid /= objList.size();
    return centroid;
}

#endif // DISTANCE_HPP
