#include "sum5.hpp"

int main(int argc, char* argv[]) { // Arguments for function main()
    if(argc < 3) {
        std::cerr << "ERRORE: Introdurre due numeri" << std::endl;
        return 1;
    }
    int n = atoi( argv[1] ); // Charachter-to-integer conversion
    int m = atoi( argv[2] );

    
    if(n > m) std::swap(n, m);

    sumSquare<int>(n, m);
    sumSquare<short>(n, m);
    sumSquare<long>(n, m);
    sumSquare<double>(n, m);
    sumSquare<long double>(n, m);

    return 0;
}
