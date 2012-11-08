#include <iostream>
#include <algorithm>
#include <random>

#include "Clustering.hpp"

int main()
{
    typedef Clustering<2, Point2D, EuclideanDistance> euclideanClustering;
    typedef Clustering<2, Point2D, AnisotropicDistance> anisotropicClustering;

    euclideanClustering eC;
    anisotropicClustering aC;

    eC.setup( 1.e-6, 3, false );
    aC.setup( 1.e-6, 3, false );

    AnisotropicDistance<Point2D>::tensor_T matrix;
    matrix << 1, 5, 0, 1;

    aC.getDistancePolicy().setTensor( matrix );

    euclideanClustering::centroidList_T centroids;

    centroids[0] << -1., 0.5;
    centroids[1] << 2., 0.5;

    eC.setInitialCentroids( centroids );
    aC.setInitialCentroids( centroids );

    euclideanClustering::objectList_T points( 5 );

//    std::random_device rd;
//    std::mt19937 gen( rd() );
//    std::normal_distribution<> n_dist( 0.5, 0.5 );
//    for( int iPoint = 0; iPoint < points.size(); iPoint++ )
//    {
//        points[iPoint] << n_dist( gen ), n_dist( gen );
//    }

    points[0] << 0., 0.;
    points[1] << 1., 0.;
    points[2] << 1., 1.;
    points[3] << 0., 1.;
    points[4] << 2., 0.5;

    eC.apply( points );
    aC.apply( points );

    centroids = eC.getCentroids();
    std::cout << "Centroids computed with euclidean norm:" << std::endl
              << "centroid 0:" << std::endl << centroids[0] << std::endl
              << "centroid 1:" << std::endl << centroids[1] << std::endl;

    centroids = aC.getCentroids();
    std::cout << "Centroids computed with anisotropic norm:" << std::endl
              << "centroid 0:" << std::endl << centroids[0] << std::endl
              << "centroid 1:" << std::endl << centroids[1] << std::endl;

    // Two {{ becaus we are initializing a subobject)
    // It is not compulsory but it is better
    // std::array<int, 3> v={{2, 1, 3}};
    // v[0] = 2; v[1] = 1; v[2] = 3;
    // std::cout << "min value at "
    //           << std::min_element( v.begin(), v.end()) - v.begin()
    //           << std::endl;
    //Eigen::Vector2d x;
    //x << 2., 3.;
    //std::cout

return 0;
}
