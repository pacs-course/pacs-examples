#ifndef CLUSTERING_HPP
#define CLUSTERING_HPP

#include <array>
#include <vector>

#include "Geo.hpp"

#include "Distance.hpp"

template <size_t N,
          typename ObjectT,
          class DistanceP>
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
    centroidList_T const & getCentroids() const { return M_centroids; }

    distance_P const & getDistancePolicy() const { return M_distancePolicy; }
    distance_P & getDistancePolicy() { return M_distancePolicy; }

    //setup
    void setup( real tol, int maxIt, bool verbose = false );

    void setInitialCentroids( centroidList_T const & centroids )
    {
        M_centroids = centroids;
    }

    // methods
    void apply( objectList_T const & objects );

private:

    bool updateCentroids();

    void computeQuality()
    {
        // TO BE IMPLEMENTED
    }

    // members
    real M_tol;
    int M_maxIt;
    bool M_verbose;

    centroidList_T M_centroids;
    objectListOnCentroid_T M_objectList;

    distance_P  M_distancePolicy;

}; // class Clustering

// TEMPLATE METHODS IMPLEMENTATION

//setup
template <size_t N, typename ObjectT, class DistanceP>
void Clustering<N, ObjectT, DistanceP>::setup( real tol, int maxIt, bool verbose )
{
    M_tol = tol;
    M_maxIt = maxIt;
    M_verbose = verbose;
}

template <size_t N, typename ObjectT, class DistanceP>
void Clustering<N, ObjectT, DistanceP>::apply( objectList_T const & objects )
{
    // iteration loop truncated at maxIt
    for( int it = 0; it < M_maxIt; it++ )
    {
        if( M_verbose )
            std::cout << "=======================" << std::endl
                      << "iteration " << it << std::endl;

        // clean up object.list
        for( size_t d = 0; d < N; d++ )
            M_objectList[d].clear();

        // loop on the object set
        for( size_t kObj = 0; kObj < objects.size(); kObj++ )
        {
            // compute the distance of the k-th object from all the centroids
            std::array<real,N> distances;
            for( size_t d = 0; d < N; d++ )
            {
                distances[d] = M_distancePolicy( objects[kObj],
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
            for( size_t d = 0; d < N; d++ )
            {
                std::cout << "centroid " << d << " has "
                          << M_objectList[d].size() << " objects" << std::endl;
                for( size_t j = 0; j < M_objectList[d].size(); j++ )
                    std::cout << M_objectList[d][j] << std::endl;
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

    computeQuality();
}

template <size_t N, typename ObjectT, class DistanceP>
bool Clustering<N, ObjectT, DistanceP>:: updateCentroids()
{
    std::array<real,N> increment;
    for( size_t d = 0; d < N; d++ )
    {
        object_T mean = M_distancePolicy.findCentroid( M_objectList[d] );

        if( M_verbose ) std::cout << "mean = " << mean << std::endl;

        increment[d] = std::fabs(
                    M_distancePolicy( M_centroids[d], mean ) );

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

#endif // CLUSTERING_HPP
