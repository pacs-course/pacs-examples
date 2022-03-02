#include <iostream>
#include "GetPot"

int main ( int argc, char* argv[] )
{
    // Read, using GetPot, the data from input
    GetPot commandLine ( argc, argv );
    double a = commandLine ( "a", 0. );
    double b = commandLine ( "b", 1. );
    int nint = commandLine ( "nint", 10 );

    std::cout << a << " " << b << " " << nint << std::endl;
    return 0;
}
