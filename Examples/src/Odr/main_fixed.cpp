#include "fun1_fixed.hpp"
#include "fun3.hpp"
#include <iostream>

int main() {
    double x = 3.0;
    std::cout << "fun1(" << x << ") = " << myspace::fun1(x) << std::endl;
    std::cout << "fun2(" << x << ") = " << myspace::fun2(x) << std::endl;
    std::cout << "fun3(" << x << ") = " << myspace::fun3(x) << std::endl;
    return 0;
}
