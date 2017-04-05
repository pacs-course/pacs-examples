#include <iostream>
#include <algorithm>
#include <random>

#include "Clustering.hpp"

int main()
{
    typedef Clustering<2, Point2D, AnisotropicDistance> anisotropicClustering;

    anisotropicClustering c;

    c.setup( 1.e-6, 3, false );

    AnisotropicDistance::tensor_T matrix;
    matrix << 3, 1, 1, 3;

    c.getDistancePolicy().setTensor( matrix );

    anisotropicClustering::centroidList_T centroids;

    centroids[0] << -1., 0.5;
    centroids[1] << 2., 0.5;

    c.setInitialCentroids( centroids );

    anisotropicClustering::objectList_T points( 6 );

    std::random_device rd;
    std::mt19937 gen( rd() );
    std::normal_distribution<> n_dist( 0.5, 0.5 );
    for( int iPoint = 0; iPoint < points.size(); iPoint++ )
    {
        points[iPoint] << n_dist( gen ), n_dist( gen );
    }

    c.apply( points );

    centroids = c.getCentroids();
    std::cout << "Centroids computed with anisotropic norm:" << std::endl
              << "centroid 0:" << std::endl << centroids[0] << std::endl
              << "centroid 1:" << std::endl << centroids[1] << std::endl;

    return 0;
}
