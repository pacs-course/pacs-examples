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

    { // A fictitious block to create a scope

        char sumT[] = "double";

        cout << "[Sum saved in a variable of type " << sumT << "]"
             << endl;
        cout << " - biggest  " << sumT << " : "
             << numeric_limits<double>::max() << endl;
        cout << " - smallest " << sumT << " : "
             << numeric_limits<double>::min() << endl;
        cout << endl;

        double sum = 0.;

        for(int i = n; i <= m; ++i) {
            sum += i * i;
        }

        cout << "the sum of squares of the integers from "
             << n << " to " << m
             << " is " << sum << endl;
    }

    { // A fictitious block to create a scope
        char sumT[] = "int";

        cout << "[Sum saved in a variable of type " << sumT << "]"
             << endl;
        cout << " - biggest  " << sumT << " : "
             << numeric_limits<double>::max() << endl;
        cout << " - smallest " << sumT << " : "
             << numeric_limits<double>::min() << endl;
        cout << endl;

        int sum = 0;

        for(int i = n; i <= m; ++i) {
            sum += i * i;
        }

        cout << "the sum of squares of the integers from "
             << n << " to " << m
             << " is " << sum << endl;
    }

    { // A ficititious block to create a scope

        char sumT[] = "long double";

        cout << "[Sum saved in a variable of type " << sumT << "]"
             << endl;
        cout << " - biggest  " << sumT << " : "
             << numeric_limits<long double>::max() << endl;
        cout << " - smallest " << sumT << " : "
             << numeric_limits<long double>::min() << endl;
        cout << endl;

        long double sum = 0;

        for(int i = n; i <= m; ++i) {
            sum += i * i;
        }

        cout << "the sum of squares of the integers from "
             << n << " to " << m
             << " is " << sum << endl;
    }

    return 0;
}
