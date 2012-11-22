#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
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

    //int fake = 1;

    for( uint j = 0; j < 20; j++ )
    {
        std::map<int,uint> b;	
        for( auto it = a.begin(); it != a.end(); ++it )
        {
	    int p = it->first;
	    uint n = it->second;
            for( uint k = 0; k < n; k++ )
            {
                int number = distribution(generator);
		//int number = fake;
		//fake = 1 - fake;
                b[ p + 2 * number - 1 ]++;
		//std::cout << "T " << p << " -> " << p + 2*number-1 << std::endl;
            }
        }

        a = b;

        std::stringstream fileName;
	fileName << "randomlap" << j+1 << ".out";
	std::ofstream fOut( fileName.str().c_str() );

        //std::copy( a.begin(), a.end(), std::ostreambuf_iterator<uint>( std::cout ) );
        std::cout << "==============================" << std::endl;
        for( auto it = b.begin(); it != b.end(); ++it )
        {
            if( std::abs( it->first ) < 20 /*&& it->second > 0*/ )
            {
                std::cout << std::setw(3) << it->first << " - " 
			  << std::setprecision(4)
			  << std::fixed << static_cast<double>(it->second) / s << "\t";
		fOut << it->first << " " << static_cast<double>(it->second) / s << std::endl;
                for( uint k = 0; k < c * (double) it->second / s; k++ )
                    std::cout << "*";
                std::cout << std::endl;
            }
        }
	fOut.close();
    }


    return 0;
}
