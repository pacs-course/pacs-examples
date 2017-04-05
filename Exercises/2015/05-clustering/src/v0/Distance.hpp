#ifndef DISTANCE_HPP
#define DISTANCE_HPP

#include "Geo.hpp"

struct EuclideanDistance
{
    typedef Point2D object_T;
    typedef std::vector<object_T> objectList_T;

    real operator()( object_T const & p1, object_T const & p2 )
    {
        return ( p1 - p2 ).norm();
    }

    object_T findCentroid( objectList_T const & objList );
};

Point2D EuclideanDistance::findCentroid( objectList_T const & objList )
{
    // compute the centroid as the barycenter of the points
    object_T centroid = object_T::Constant( 0. );
    for( size_t kObj = 0; kObj < objList.size(); kObj++ )
        centroid += objList[kObj];
    centroid /= objList.size();
    return centroid;
}

#endif // DISTANCE_HPP
