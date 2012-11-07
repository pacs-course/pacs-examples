#ifndef CLUSTERING_HPP
#define CLUSTERING_HPP

#include <Eigen/Dense>

typedef double real;

typedef Eigen::Vector2d Point2D;
//struct Point2D {};

struct EuclideanDistance
{
  static real computeDistance( Point2D const & p1, Point2D const & p2 )
  {
    return ( p1 - p2 ).norm();
  }
};


#include <array>
#include <array>

template <size_t N,
          typename ObjectT,
          typename DistanceP>
class Clustering
{
public:
  typedef ObjectT object_T;
  typedef DistanceP distance_P;
  typedef std::array<object_T,N> centroidList_T;
  typedef std::vector<object_T> objectList_T;
  typedef std::array<objectList_T,N> objectListOnCentroid_T;

  // constructors
  Clustering() = default;
  ~Clustering() = default;
  Clustering( Clustering const & ) = delete;
  Clustering & operator=( Clustering const & ) = delete;

  // getters
  centroidList_T const & getCentroids() { return M_centroids; }

  //setup
  void setup() {}
  void setInitialCentroids( centroidList_T const & centroids )
  {
    M_centroids = centroids;
  }

  real apply( objectList_T const & objects )
  {
    for( size_t i = 0; i < objects.size(); i++ )
    {
      std::array<real,N> distances;
      for( size_t k = 0; k < N; k++ )
      {
        distances[k] = distance_P::computeDistance( objects[i],
                                                    M_centroids[k] );
      }
      int minPos = std::min_element( distances.begin(), distances.end() )
                   - distances.begin();
      M_objectList[minPos].push_back( objects[i] );
    }

    for( size_t k = 0; k < N; k++ )
    {
      std::cout << k << " > " << M_objectList[k].size() << std::endl;
      for( size_t j = 0; j < M_objectList[k].size(); j++ )
        std::cout << M_objectList[k][j] << std::endl;
    }

    updateCentroids();

    return DistanceP::computeDistance( M_centroids[0], M_centroids[1] );
  }

private:
  void updateCentroids()
  {
    for( size_t k = 0; k < N; k++ )
    {
        object_T mean = object_T::Constant( 0. );
      for( size_t j = 0; j < M_objectList[k].size(); j++ )
        mean += M_objectList[k][j];
      mean /= M_objectList[k].size();
      std:: cout << "mean = " << mean << std::endl;
      M_centroids[k] = mean;
    }
  }

  centroidList_T M_centroids;
  objectListOnCentroid_T M_objectList;

}; // class Clustering

#endif //CLUSTERING_HPP
