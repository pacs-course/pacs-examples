/*! \file sum0.cpp

\brief This program aims at evaluating the sum of the powers of integers taken
between two user-defined values
*/

#include <iostream>

int main() {
    int n, m;
    std::cout << "Insert 2 integers:" << std::endl;
    std::cin >> n >> m;

    if(n > m) { // Swap variables if necessary
        int temp = n;
        n = m;
        m = temp;
    }

    double sum = 0.;

    for(int i = n; i <= m; ++i) {
        sum += i * i;
    }

    std::cout << "The sum of integer squares from "
              << n << " to " << m
              << " is " << sum << std::endl;

    return 0;
}
