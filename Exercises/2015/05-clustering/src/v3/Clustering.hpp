#ifndef CLUSTERING_HPP
#define CLUSTERING_HPP

#include <array>
#include <vector>
#include <fstream>
#include <iomanip>

#include "Geo.hpp"

#include "Distance.hpp"

template <size_t N,
          typename ObjectT,
          template<class> class DistanceP>
class Clustering
{
public:
    typedef ObjectT object_T;
    typedef DistanceP<ObjectT> distance_P;
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

    void printGnuplot( int const i );

private:

    bool updateCentroids();

    void computeQuality();

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
template <size_t N, typename ObjectT, template<class> class DistanceP>
void Clustering<N, ObjectT, DistanceP>::setup( real tol, int maxIt, bool verbose )
{
    M_tol = tol;
    M_maxIt = maxIt;
    M_verbose = verbose;
}

template <size_t N, typename ObjectT, template<class> class DistanceP>
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

        printGnuplot( it );

        if ( !isIncremented )
        {
            if ( M_verbose )
                std::cout << "iteration interrupted at " << it << std::endl;
            break;
        }
    }

    computeQuality();
}

template <size_t N, typename ObjectT, template<class> class DistanceP>
bool Clustering<N, ObjectT, DistanceP>::updateCentroids()
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

template <size_t N, typename ObjectT, template<class> class DistanceP>
void Clustering<N, ObjectT, DistanceP>::printGnuplot( int const i )
{
/* output example
# centroids
# x y (fixed style)
0. 0.5 -1
1. 0.5 -1

# points
# x y cluster
0. 0. 0
1. 0. 0
0. 1. 0

0.5 0. 1
0.5 1. 1
*/

    std::stringstream datFileName;
    datFileName << "cluster" << std::setfill('0')
                << std::setw( std::floor(std::log10(M_maxIt) ) )
                << i << ".dat";
    std::ofstream datFile( datFileName.str().c_str() );

    datFile << "# centroids\n# x y (fixed style)\n";
    for( size_t d = 0; d < N; d++ )
    {
        datFile << M_centroids[d][0] << " "
                << M_centroids[d][1] << " -1\n";
    }
    datFile << "\n" << std::endl;

    datFile << "# points\n# x y cluster\n";
    for( size_t d = 0; d < N; d++ )
    {
        for( size_t kObj = 0; kObj < M_objectList[d].size(); kObj++ )
        {
            datFile << M_objectList[d][kObj][0] << " "
                    << M_objectList[d][kObj][1] << " " << d << "\n";
        }
        datFile << "\n" << std::endl;
    }
    datFile.close();

    std::stringstream pngFileName;
    pngFileName << "cluster" << std::setfill('0')
                << std::setw( std::floor(std::log10(M_maxIt) ) )<< i << ".png";

    std::ofstream gpFile( "cluster.gnuplot" );
    gpFile << "set terminal png size 800,640\n"
           << "set output \'" << pngFileName.str() << "\'\n"
           << "set xrange [-0.1:1.1]\n"
           << "set yrange [-0.1:1.1]\n"
           << "set pointsize 2.5\n"
           << "plot \'" << datFileName.str() << "\' w p palette title \'\',\\\n"
           << "\'<(head -n " << N + 2 << " " << datFileName.str()
           << ")\'w p pt 7 title \'\'" << std::endl;

    system( "gnuplot cluster.gnuplot" );
}

template <size_t N, typename ObjectT, template<class> class DistanceP>
void Clustering<N, ObjectT, DistanceP>::computeQuality()
{
  real sum = 0.;
  for( size_t d = 0; d < N; d++ )
  {
    std::for_each( M_objectList[d].begin(),
                   M_objectList[d].end(),
                   [&]( object_T const & p )
                   { sum += M_distancePolicy( M_centroids[d], p ); } );
  }
  std::cout << "Quality = " << sum << std::endl;
}

#endif // CLUSTERING_HPP
