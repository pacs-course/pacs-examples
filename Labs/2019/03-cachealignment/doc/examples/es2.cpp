#include <iostream>
#include "GetPot"

int main()
{
    // Read, using GetPot, the data from input
    GetPot fileData("data");

    double a = fileData ( "integration/domain/a", 0.);
    double b = fileData ( "integration/domain/b", 1.);
    int nint = fileData ( "integration/mesh/nint", 10);

    std::cout << a << " " << b << " " << nint << std::endl;
    return 0;
}
