#include <iostream>
#include <algorithm>
#include <random>
#include <map>


typedef unsigned int uint;

int main()
{
    std::default_random_engine generator;
    std::discrete_distribution<int> distribution { 1, 1 };

    std::map<int,uint> a;

    uint const s = 10000;
    uint const c = 100;

    a[0] = s;

    for( uint j = 0; j < 100; j++ )
    {
        for( uint i = 0; i < 10; i++ )
        {
            for( auto it = a.begin(); it != a.end(); ++it )
            {
                for( uint k = 0; k < it->second; k++ )
                {
                    int number = distribution(generator);
                    a[ it->first ]--;
                    a[ it->first + 2 * number - 1 ]++;
                }
            }
        }
        //std::copy( a.begin(), a.end(), std::ostreambuf_iterator<uint>( std::cout ) );
        std::cout << "==============================" << std::endl;
        for( auto it = a.begin(); it != a.end(); ++it )
        {
            if( std::abs( it->first ) < 10 /*&& it->second > 0*/ )
            {
                std::cout << it->first << " > " << (double) it->second / s << "\t\t";
                for( uint k = 0; k < c * (double) it->second / s; k++ )
                    std::cout << "*";
                std::cout << std::endl;
            }
        }
    }


    return 0;
}
