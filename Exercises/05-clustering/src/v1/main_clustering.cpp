#include <iostream>
#include <algorithm>
#include <random>

#include "Clustering.hpp"

int main()
{
    typedef Clustering<2, Point2D, EuclideanDistance> euclideanClustering;

    euclideanClustering eC;

    eC.setup( 1.e-6, 3, false );

    // generate a random set of points
    std::random_device rd;
    std::mt19937 gen( rd() );
    std::normal_distribution<> n_dist( 0.5, 0.5 );

    euclideanClustering::centroidList_T centroids;

    centroids[0] << n_dist( gen ), n_dist( gen );
    centroids[1] << n_dist( gen ), n_dist( gen );

    eC.setInitialCentroids( centroids );

    euclideanClustering::objectList_T points( 6 );
    for( int iPoint = 0; iPoint < points.size(); iPoint++ )
    {
        points[iPoint] << n_dist( gen ), n_dist( gen );
    }

    // use a fized set of points
    // points[0] << 0., 0.;
    // points[1] << 1., 0.;
    // points[2] << 1., 1.;
    // points[3] << 0., 1.;
    // points[4] << 2., 0.;
    // points[5] << 2., 1.;

    eC.apply( points );

    centroids = eC.getCentroids();
    std::cout << "Centroids computed with euclidean norm:" << std::endl
              << "centroid 0:" << std::endl << centroids[0] << std::endl
              << "centroid 1:" << std::endl << centroids[1] << std::endl;

    return 0;
}

