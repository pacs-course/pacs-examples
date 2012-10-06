#include <iostream>
#include <cstdlib>
#include <limits>

using namespace std;

int main(int argc, char* argv[]) { // Arguments for function main()
    if(argc < 3) {
        cerr << "ERROR: the program should be called with 2 integers as arguments" << endl;
        return 1;
    }
    int n = atoi( argv[1] ); // Charachter-to-integer conversion
    int m = atoi( argv[2] );

    if(n > m) { // Swap variables if necessary
        int temp = n;
        n = m;
        m = temp;
    }

    char sumT[] = "int";

    cout << "[Sum saved in a variable of type " << sumT << "]"
         << endl;
    cout << " - biggest  " << sumT << " : "
         << numeric_limits<int>::max() << endl;
    cout << " - smallest " << sumT << " : "
         << numeric_limits<int>::min() << endl;
    cout << endl;

    int sum = 0;

    for(int i = n; i <= m; ++i) {
        sum += i * i;
    }

    cout << "the sum of squares of the integers from "
         << n << " to " << m
         << " is " << sum << endl;

    return 0;
}
