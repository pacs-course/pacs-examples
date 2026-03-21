#pragma once
namespace myspace {
    //! Correctly defined as inline in a header to satisfy ODR across TUs.
    inline double fun1(double x) {
        return x + 1; // Same behavior as the failing example
    }

    //! Header definition is safe because this function is inline.
    inline double fun2(double x) {
        return x * x; // Same behavior as the failing example
    }
}
