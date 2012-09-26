#include <iostream>
#include <vector>
#include <cstdlib>

using namespace std; // Avoid using scope operator to access namespace std

int main(int argc, char* argv[])
{ // Arguments for function main()
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
    psum.reserve(m - n + 1); // Reserve m - n + 1 elements

    for(int i = n; i <= m; ++i)
    {
        sum += i * i;
        psum.push_back(sum);
        cout << "Size " << psum.size() << endl;
        cout << "Capacity " << psum.capacity() << endl;
    }

    cout << "La somma dei quadrati degli interi da "
         << n << " a " << m
         << " e' " << sum << endl;

    cout << "Vettore delle somme parziali : " << endl;

    for(vector<double>::iterator p = psum.begin(); p != psum.end(); ++p)
    {
        cout << *p << " " << flush;
    }

    cout << endl;

    return 0;
}
