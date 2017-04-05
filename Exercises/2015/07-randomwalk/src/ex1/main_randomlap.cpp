#include <iostream>

#include "Distributions.hpp"
#include "RandomWalk.hpp"

int main()
{

    RandomWalk<discrete_distribution<int>> rw( {1,1}, 10000, SCREEN | GNUPLOT );
    //RandomWalk<FakeDistribution> rw( {1,1}, 10000, SCREEN | GNUPLOT );

    for( uint time = 0; time < 20; time++ )
    {
        rw.step( time );
    }

    return 0;
}
