#include <iostream>
#include <vector>
#include <cstdlib>

using namespace std; // Avoid using qualifier std:: to access namespace std

int main(int argc, char* argv[]) { // Arguments for function main()
    if(argc < 3)
    {
        cerr << "ERRORE: Introdurre due numeri" << endl;
        return 1;
    }

    int n = atoi( argv[1] ); // Character-to-integer conversion
    int m = atoi( argv[2] );

    if(n > m)
    { // Swap variables if necessary
        int temp = n;
        n = m;
        m = temp;
    }

    double sum = 0.;

    vector<double> psum;
    psum.resize(m - n + 1); // Resize to hold m - n + 1 elements
    cout << "size " << psum.size() << endl;
    cout << "capacity " << psum.capacity() << endl;

    for(int i = n; i <= m; ++i)
    {
        sum += i * i;
        psum[i - n] = sum;
    }

    cout << "La somma dei quadrati degli interi da "
         << n << " a " << m
         << " e' " << sum << endl;

    cout << "Vettore delle somme parziali : " << endl;

    for(int i = 0; i < psum.size(); ++i)
    {
        cout << psum[i] << " " << flush;
    }

    cout << endl;

    return 0;
}
