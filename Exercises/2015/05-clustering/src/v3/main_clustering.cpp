#include <iostream>
#include <algorithm>
#include <random>

#include "Clustering.hpp"

int main()
{
    typedef Clustering<4, Point2D, EuclideanDistance> euclideanClustering;
    typedef Clustering<4, Point2D, AnisotropicDistance> anisotropicClustering;

    euclideanClustering eC;
    anisotropicClustering aC;

    eC.setup( 1.e-6, 100, false );
    aC.setup( 1.e-6, 100, false );

    AnisotropicDistance<Point2D>::tensor_T matrix;
    matrix << 3, 1, 1, 3;

    aC.getDistancePolicy().setTensor( matrix );

    std::random_device rd;
    std::mt19937 gen( rd() );
    std::normal_distribution<> n_dist( 0.5, 0.3 );

    euclideanClustering::centroidList_T centroids;

    for( int iC = 0; iC < centroids.size(); iC++ )
    {
        centroids[iC] << n_dist( gen ), n_dist( gen );
    }

//    centroids[0] << -1., 0.5;
//    centroids[1] << 2., 0.5;

    eC.setInitialCentroids( centroids );
    aC.setInitialCentroids( centroids );

    euclideanClustering::objectList_T points( 1000 );
    for( int iPoint = 0; iPoint < points.size(); iPoint++ )
    {
        points[iPoint] << n_dist( gen ), n_dist( gen );
    }

//    points[0] << 0., 0.;
//    points[1] << 1., 0.;
//    points[2] << 1., 1.;
//    points[3] << 0., 1.;
//    points[4] << 2., 0.;
//    points[5] << 2., 1.;

    eC.apply( points );
    //aC.apply( points );

    centroids = eC.getCentroids();
    std::cout << "Centroids computed with euclidean norm:" << std::endl
              << "centroid 0:" << std::endl << centroids[0] << std::endl
              << "centroid 1:" << std::endl << centroids[1] << std::endl;

    centroids = aC.getCentroids();
    std::cout << "Centroids computed with anisotropic norm:" << std::endl
              << "centroid 0:" << std::endl << centroids[0] << std::endl
              << "centroid 1:" << std::endl << centroids[1] << std::endl;

    //system("ffmpeg -i cluster%02d.png cluster.avi -r 1");

    return 0;
}
