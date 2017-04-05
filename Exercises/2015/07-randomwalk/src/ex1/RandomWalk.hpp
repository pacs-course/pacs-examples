#ifndef RANDOMWALK_HPP
#define RANDOMWALK_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <map>

#include "Util.hpp"

using namespace std;

template <typename DistributionT>
class RandomWalk
{
public:
    typedef map<int,uint> storage_T;
    typedef DistributionT distribution_T;

    explicit RandomWalk( initializer_list<double> il,
                         uint nsamples,
                         oFlag_T outputlevel = NONE );

    void step( const uint timestep );

private:
    void print( const uint timestep );

    const uint M_nsamples;
    storage_T M_data;
    default_random_engine M_generator;
    distribution_T M_dist;
    const Rescaler M_rescaler;
    oFlag_T M_outputlevel;
};

template <typename DistributionT>
RandomWalk<DistributionT>::RandomWalk( initializer_list<double> il,
                                       uint nsamples,
                                       oFlag_T outputlevel ):
    M_nsamples( nsamples ),
    M_dist( il ),
    M_rescaler( il.size() - 1 ),
    M_outputlevel( outputlevel )
{
    M_data[0] = nsamples;
}

template <typename DistributionT>
void RandomWalk<DistributionT>::step( const uint timestep )
{
    storage_T newData;
    for( auto i: M_data )
    {
        for( uint k = 0; k < i.second; k++ )
        {
            int number = M_dist(M_generator);
            newData[ i.first + M_rescaler( number ) ]++;
        }
    }
    M_data = newData;

    print(timestep);
}

template <typename DistributionT>
void RandomWalk<DistributionT>::print( const uint timestep )
{
    if( M_outputlevel & GNUPLOT )
    {
        stringstream fileName;
        fileName << "randomlap" << setw(2) << setfill('0')
                 << timestep+1 << ".out";
        ofstream fOut( fileName.str().c_str() );
        for( auto i: M_data )
        {
            float pct = static_cast<double>(i.second) / M_nsamples;
            fOut << i.first << " " << pct << endl;
        }
        fOut.close();
    }

    if( M_outputlevel & SCREEN )
    {
        cout << "==============================" << endl;
        for( auto i: M_data )
        {
            if( abs( i.first ) < 20 )
            {
                float pct = static_cast<double>(i.second) / M_nsamples;
                cout << setw(3) << i.first << " | " << setprecision(4)
                     << fixed << pct << " |";
                for( uint k = 0; k < 100 * pct; k++ )
                    cout << "*";
                cout << endl;
            }
        }
    }
}

#endif // RANDOMWALK_HPP
