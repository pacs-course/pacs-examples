#ifndef DISTRIBUTIONS_HPP
#define DISTRIBUTIONS_HPP

#include <random>
#include <initializer_list>

// this class mimics the behaviour of a random engine, but it distributes
// evenly between the range 0...N, where N is the size of the initialize list
// that is passed to the constructor. The weights are non used at all!
class FakeDistribution
{
public:

    FakeDistribution( std::initializer_list<double> il ):
        M_counter(0),
        M_size( il.size() )
    {}

    uint operator()( std::default_random_engine & /*e*/ )
    {
        M_counter = (M_counter + 1)%M_size;
        return M_counter;
    }

private:
    uint M_counter;
    uint M_size;
};

#endif // DISTRIBUTIONS_HPP
