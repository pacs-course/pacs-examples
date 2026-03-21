#include "fun1.hpp"
namespace myspace {
    //! a function that calls fun1 and fun2, defined in the header file.
    double fun3(double x) {
        return 2. + myspace::fun1(x)-myspace::fun2(x); // Just an example implementation
    }
}