#ifndef UTIL_HPP
#define UTIL_HPP

// typedef to shorten the use of unsigned
typedef unsigned int uint;

// this functor translates an integer i in [0,N) in the range [-1,1]
class Rescaler
{
public:
    explicit Rescaler( const uint max ):M_max( max ) {}

    int operator()( const uint i ) const
    {
        return ( 2 * i ) / M_max - 1;
    }

private:
    const uint M_max;
};

// output flags that are used to activate print to screen and/or file
typedef short oFlag_T;
const oFlag_T NONE    = 0x00; // binary: 000
const oFlag_T SCREEN  = 0x01; // binary: 010
const oFlag_T GNUPLOT = 0x02; // binary: 100

#endif // UTIL_HPP
