
#include <iostream>     // include a library for input and output 
#include <cmath>      

using namespace std;

int main() {

double d = 10.5;
double e;
cout << "return  : " << modf(d, &e) << "\n";
cout << "e is: " << e << "\n";

int exp;
cout << "mantissa: " << frexp(d, &exp) << "\n";
cout << "exp is: " << exp << "\n";
d = 0;
cout << "mantissa: " << frexp(d, &exp) << "\n";
cout << "exp is: " << exp << "\n";

}
