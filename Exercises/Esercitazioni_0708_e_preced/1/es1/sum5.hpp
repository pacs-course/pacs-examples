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

    std::cout << "[Somma memorizzata in una variabile di tipo " << sumT << "]" 
              << std::endl;
    std::cout << " - il piu' grande  " << sumT << " : "
              << std::numeric_limits<T>::max() << std::endl;
    std::cout << " - il piu' piccolo " << sumT << " : "
              << std::numeric_limits<T>::min() << std::endl;
    std::cout << " - # di byte per   " << sumT << " : "
              << sizeof(T) << std::endl;

    T sum = 0;

    for(int i = n; i <= m; ++i) {
        sum += i * i;
    }

    std::cout << "La somma dei quadrati degli interi da " << n << " a " << m 
         << " e' " << sum << std::endl;

    std::cout << "**************************************************"
              << std::endl;
}
