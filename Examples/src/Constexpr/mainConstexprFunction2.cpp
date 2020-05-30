#include "twoFunctions.hpp"
#include <iostream>
int main()
{
    double a;
    //std::cin >> a;
    // Here the compiler cannot resolve statically since a is not a constant expression, but a variable.
    // The function  will be
    // inlined (since constexpr implies inline) but not completely
    // resolved at compile time
    auto x = cubeconst (a);
}

