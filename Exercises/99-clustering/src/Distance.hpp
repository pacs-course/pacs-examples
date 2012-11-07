#ifndef DISTANCE_HPP
#define DISTANCE_HPP

struct Distance
{
    virtual real computeDistance( Point2D const & p1, Point2D const & p2 ) = 0;
};

struct EuclideanDistance: public Distance
{
    real computeDistance( Point2D const & p1, Point2D const & p2 )
    {
        return ( p1 - p2 ).norm();
    }
};

#endif // DISTANCE_HPP
