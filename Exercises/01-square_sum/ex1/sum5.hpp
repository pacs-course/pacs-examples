#include <iostream>
#include <limits>
#include <algorithm>

/*
  A template struct to get the name of a type
*/

template<typename T>
struct name {};

template<>
struct name<double> {
    static std::string get() {
        return "double";
    }
};

template<>
struct name<int> {
    static std::string get() {
        return "int";
    }
};

template<>
struct name<long double> {
    static std::string get() {
        return "long double";
    }
};

template<>
struct name<long> {
    static std::string get() {
        return "long";
    }
};

template<>
struct name<short> {
    static std::string get() {
        return "short";
    }
};

/* 
   A template function to compute the sum of the squares of the integers 
   between n and m
*/

template<typename T>
void sumSquare(int n, int m) {
    std::string sumT = name<T>::get();

    std::cout << "[Sum saved in a variable of type " << sumT << "]"
              << std::endl;
    std::cout << " - biggest  " << sumT << " : "
              << std::numeric_limits<T>::max() << std::endl;
    std::cout << " - smallest " << sumT << " : "
              << std::numeric_limits<T>::min() << std::endl;
    std::cout << " - # of bytes for " << sumT << " : "
              << sizeof(T) << std::endl;

    T sum = 0;

    for(int i = n; i <= m; ++i) {
        sum += i * i;
    }

    std::cout << "The sum of integer squares from "
              << n << " to " << m
              << " is " << sum << std::endl;

    std::cout << "**************************************************"
              << std::endl;
}
