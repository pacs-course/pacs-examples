#pragma once
namespace myspace {
    //! A simple function that takes a double and returns a double.
    /*!
    This is an error: this function is defined in a header file without being declared as inline, or constexpr, 
    which can lead to multiple definition errors if this header is included 
    in multiple translation units.
    */ 
    double fun1(double x) {
        return x + 1; // Just an example implementation
    }
    //! Another function that takes a double and returns a double, defined inline.
    inline double fun2(double x) {
        return x * x; // Just an example implementation
    }
}