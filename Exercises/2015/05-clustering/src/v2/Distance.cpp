#include "Distance.hpp"

Point2D EuclideanDistance::findCentroid( objectList_T const & objList )
{
    object_T centroid = object_T::Constant( 0. );
    for( size_t kObj = 0; kObj < objList.size(); kObj++ )
        centroid += objList[kObj];
    centroid /= objList.size();
    return centroid;
}

Point2D AnisotropicDistance::findCentroid( objectList_T const & objList )
{
    object_T centroid = object_T::Constant( 0. );
    for( size_t kObj = 0; kObj < objList.size(); kObj++ )
        centroid += M_tensor * objList[kObj];
    centroid = M_tensor.inverse() * centroid;
    centroid /= objList.size();
    return centroid;
}

