#include <iostream>
#include <string>
#include "GetPot"

int main()
{
    // Read, using GetPot, the data from input
    GetPot fileData ("data");

    const std::string globalsection = "integration/";
    const std::string section1 = globalsection + "domain/";
    const std::string section2 = globalsection + "mesh/";

    double a = fileData ( (section1 + "a").data(), 0. );
    double b = fileData ( (section1 + "b").data(), 1. );
    int nint = fileData ( (section2 + "nint").data(), 10 );

    std::cout << a << " " << b << " " << nint << std::endl;
    return 0;
}
