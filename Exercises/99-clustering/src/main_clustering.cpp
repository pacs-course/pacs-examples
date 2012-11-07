#include <iostream>
#include <algorithm>

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

  euclideanClustering::objectList_T points( 4 );

  points[0] << 0., 0.;
  points[1] << 1., 0.;
  points[2] << 1., 1.;
  points[3] << 0., 1.;

  real dist = c.apply( points );

  std::cout << "centroid distance = " << dist << std::endl;

  std::array<int, 3> v({2, 1, 3});
  //v[0] = 2; v[1] = 1; v[2] = 3;
  std::cout << "min value at "
            << std::min_element( v.begin(), v.end()) - v.begin()
            << std::endl;
  //Eigen::Vector2d x;
  //x << 2., 3.;
  //std::cout

  return 0;
}
