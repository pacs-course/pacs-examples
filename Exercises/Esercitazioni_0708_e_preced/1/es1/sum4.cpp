#include <iostream>
using namespace std;

main(int argc, char* argv[]) { // Arguments for function main()
    if(argc < 3) {
        cerr << "ERRORE: Introdurre due numeri" << endl;
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

        cout << "[Somma memorizzata in una variabile di tipo " << sumT << "]" 
             << endl;
        cout << " - il piu' grande  " << sumT << " : "
             << numeric_limits<double>::max() << endl;
        cout << " - il piu' piccolo " << sumT << " : "
             << numeric_limits<double>::min() << endl;
        cout << endl;

        double sum = 0.;

        for(int i = n; i <= m; ++i) {
            sum += i * i;
        }

        cout << "La somma dei quadrati degli interi da " << n << " a " << m 
             << " e' " << sum << endl;
    }

    { // A fictitious block to create a scope
        char sumT[] = "int";

        cout << "[Somma memorizzata in una variabile di tipo " << sumT << "]" 
             << endl;
        cout << " - il piu' grande  " << sumT << " : "
             << numeric_limits<int>::max() << endl;
        cout << " - il piu' piccolo " << sumT << " : "
             << numeric_limits<int>::min() << endl;
        cout << endl;

        int sum = 0;

        for(int i = n; i <= m; ++i) {
            sum += i * i;
        }

        cout << "La somma dei quadrati degli interi da " << n << " a " << m 
             << " e' " << sum << endl;
    }

    { // A ficititious block to create a scope

        char sumT[] = "long double";

        cout << "[Somma memorizzata in una variabile di tipo " << sumT << "]" 
             << endl;
        cout << " - il piu' grande  " << sumT << " : "
             << numeric_limits<long double>::max() << endl;
        cout << " - il piu' piccolo " << sumT << " : "
             << numeric_limits<long double>::min() << endl;
        cout << endl;

        long double sum = 0;

        for(int i = n; i <= m; ++i) {
            sum += i * i;
        }

        cout << "La somma dei quadrati degli interi da " << n << " a " << m 
             << " e' " << sum << endl;
    }

    return 0;
}
