#ifndef MYFUNCTOR_HPP
#define MYFUNCTOR_HPP

struct myFunctor
{
    myFunctor() = default;

    double operator() ( const double & x )
    {
        return x + M_a;
    }

private:

    double M_a{9.};

};

#endif
