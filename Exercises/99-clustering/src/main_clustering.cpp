#include <iostream>
#include <algorithm>
#include <random>

#include "Clustering.hpp"

int main()
{
    typedef Clustering<2, Point2D, EuclideanDistance> euclideanClustering;

    euclideanClustering c;

    c.setup( 1.e-6, 3, true );

    euclideanClustering::centroidList_T centroids;

    centroids[0] << -1., 0.5;
    centroids[1] << 2., 0.5;

    c.setInitialCentroids( centroids );

    std::random_device rd;
    std::mt19937 gen( rd() );
    std::normal_distribution<> n_dist( 0.5, 0.5 );

    euclideanClustering::objectList_T points( 4 );

    for( int iPoint = 0; iPoint < points.size(); iPoint++ )
    {
        points[iPoint] << n_dist( gen ), n_dist( gen );
    }

    real dist = c.apply( points );

    std::cout << "centroid distance = " << dist << std::endl;

    // Two {{ becaus we are initializing a subobject)
    // It is not compulsory but it is better
    std::array<int, 3> v={{2, 1, 3}};
    //v[0] = 2; v[1] = 1; v[2] = 3;
    std::cout << "min value at "
              << std::min_element( v.begin(), v.end()) - v.begin()
              << std::endl;
    //Eigen::Vector2d x;
    //x << 2., 3.;
    //std::cout

return 0;
}
