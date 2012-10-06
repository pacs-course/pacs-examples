#include <iostream>
#include <vector>
#include <cstdlib>

using namespace std; // Avoid using scope operator to access namespace std

int main(int argc, char* argv[]) { // Arguments for function main()
    if(argc < 3) {
        cerr << "ERROR: the program should be called with 2 integers as arguments" << endl;
        return 1;
    }

    int n = atoi( argv[1] ); // Character-to-integer conversion
    int m = atoi( argv[2] );

    if(n > m) { // Swap variables if necessary
        int temp = n;
        n = m;
        m = temp;
    }

    double sum = 0.;

    typedef vector<double> psumT;
    psumT psum;
    psum.resize(m - n + 1); // Resize to hold m - n + 1 elements

    for(int i = n; i <= m; ++i) {
        sum += i * i;
        psum[i - n] = sum;
    }

    cout << "the sum of squares of the integers from "
         << n << " to " << m
         << " is " << sum << endl;

    cout << "Partial sum vector: " << endl;

    for(psumT::iterator p = psum.begin(); 
        p != psum.end(); ++p) 
        cout << *p << " " << flush;
    cout << endl;

    cout << "Partial sum vector (reversed): " << endl;

    for(psumT::reverse_iterator p = psum.rbegin();
        p != psum.rend(); ++p)
        cout << *p << " " << flush;

    cout << endl;

    psumT psum10(10);
    psum10.assign(psum.begin(), psum.begin() + 10);

    cout << "Partial sum vector (first 10): " << endl;

    for(psumT::iterator p = psum10.begin();
        p != psum10.end() - 1; ++p)
        cout << *p << " " << flush;
    cout << endl;

    return 0;
}
