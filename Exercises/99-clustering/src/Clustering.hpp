#ifndef CLUSTERING_HPP
#define CLUSTERING_HPP

#include <Eigen/Dense>

typedef double real;

typedef Eigen::Vector2d Point2D;
//struct Point2D {};

struct EuclideanDistance
{
    real computeDistance( Point2D const & p1, Point2D const & p2 )
    {
        return ( p1 - p2 ).norm();
    }
};


#include <array>
#include <vector>

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
    void setup( real tol, int maxIt, bool verbose = false )
    {
        M_tol = tol;
        M_maxIt = maxIt;
        M_verbose = verbose;
    }

    void setInitialCentroids( centroidList_T const & centroids )
    {
        M_centroids = centroids;
    }

    // methods
    real apply( objectList_T const & objects )
    {
        // iteration loop truncated at maxIt
        for( int it = 0; it < M_maxIt; it++ )
        {
            if( M_verbose )
                std::cout << "=======================" << std::endl
                          << "iteration " << it << std::endl;
            // loop on the object set
            for( size_t kObj = 0; kObj < objects.size(); kObj++ )
            {
                // compute the distance of the k-th object from all the centroids
                std::array<real,N> distances;
                for( size_t d = 0; d < N; d++ )
                {
                    distances[d] = M_distancePolicy.computeDistance( objects[kObj],
                                                                     M_centroids[d] );
                }

                // find the minimum distance
                int minPos = std::min_element( distances.begin(), distances.end() )
                        - distances.begin();

                // insert the object in the list of the closest centroid
                M_objectList[minPos].push_back( objects[kObj] );
            }

            if( M_verbose )
            {
                for( size_t k = 0; k < N; k++ )
                {
                    std::cout << k << " > " << M_objectList[k].size() << std::endl;
                    for( size_t j = 0; j < M_objectList[k].size(); j++ )
                        std::cout << M_objectList[k][j] << std::endl;
                }
            }

            bool const isIncremented = updateCentroids();
            if ( !isIncremented )
            {
                if ( M_verbose )
                    std::cout << "iteration interrupted at " << it << std::endl;
                break;
            }
        }
        return M_distancePolicy.computeDistance( M_centroids[0], M_centroids[1] );
    }

private:

    bool updateCentroids()
    {
        std::array<real,N> increment;
        for( size_t d = 0; d < N; d++ )
        {
            object_T mean = object_T::Constant( 0. );
            for( size_t kObj = 0; kObj < M_objectList[d].size(); kObj++ )
                mean += M_objectList[d][kObj];
            mean /= M_objectList[d].size();
            std::cout << "mean = " << mean << std::endl;
            increment[d] = std::fabs(
                        M_distancePolicy.computeDistance( M_centroids[d], mean ) );
            // update centroids value
            M_centroids[d] = mean;
        }
        real incrementMean;
        std::for_each( increment.begin(),
                       increment.end(),
                       [&]( real i ){ incrementMean += i; } );
    incrementMean /= N;
    return incrementMean > M_tol;
}

// members
real M_tol;
int M_maxIt;
bool M_verbose;

centroidList_T M_centroids;
objectListOnCentroid_T M_objectList;

distance_P M_distancePolicy;

}; // class Clustering

#endif //CLUSTERING_HPP
