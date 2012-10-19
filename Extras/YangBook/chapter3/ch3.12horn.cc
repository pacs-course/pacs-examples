double eval(double* a, int n, double x) {      // straightforward evaluation
  double sum = 0;
  for (int k = n; k >= 0; k--) {
    double xpowerk= 1;
    for (int i = 1; i <= k; i++) {             // compute x to the power of k
      xpowerk *= x;
    }
    sum += a[k]*xpowerk;                       // add each term to sum
  }
  return sum;
}  // end of eval()  

double horner(double* a, int n, double x) {    // Horner's algorithm
  double u = a[n];
  for (int i = n - 1; i >= 0; i--) u = u*x + a[i];
  return u;
}  // end of horner()


#include <iostream>
#include <cmath>

using namespace std;

int main() {
  double a[9] = {1, - 8, 28, - 56, 70, - 56, 28, - 8, 1};
  for (double x = 0.99999; x <= 1.0001; x += 1.0e-5) {
/*
    cout.width(20);
    cout << eval(a, 8, x);                        // straightforward evaluation
    cout.width(20);
    cout << horner(a, 8, x);                      // Horner's algorithm
    cout.width(20);
    cout << pow(x-1, 8) << endl;                  // direct answer
*/
    cout.width(20);
    cout << 1.0e10*(pow(x-1, 8) - eval(a, 8, x)) << endl;
    cout.width(20);
    cout << 1.0e10*(pow(x-1, 8) - horner(a, 8, x)) << endl;   
  }
}  // end of main()
