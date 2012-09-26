#include <iostream>
#include <vector>
#include <cstdlib>

using namespace std; // Avoid using scope operator to access namespace std

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

    typedef vector<double> psumT;
    psumT psum;
    psum.resize(m - n + 1); // Resize to hold m - n + 1 elements

    for(int i = n; i <= m; ++i)
    {
        sum += i * i;
        psum[i - n] = sum;
    }

    cout << "La somma dei quadrati degli interi da "
         << n << " a " << m
         << " e' " << sum << endl;

    cout << "Vettore delle somme parziali : " << endl;

    for(psumT::iterator p = psum.begin(); p != psum.end(); ++p)
    {
        cout << *p << " " << flush;
    }
    cout << endl;

    cout << "Vettore delle somme parziali (al contrario) : " << endl;

    for(psumT::reverse_iterator p = psum.rbegin(); p != psum.rend(); ++p)
    {
        cout << *p << " " << flush;
    }
    cout << endl;

    psumT psum10(10);
    psum10.assign(psum.begin(), psum.begin() + 10);

    for(psumT::iterator p = psum10.begin(); p != psum10.end(); ++p)
    {
        cout << *p << " " << flush;
    }
    cout << endl;

    return 0;
}
