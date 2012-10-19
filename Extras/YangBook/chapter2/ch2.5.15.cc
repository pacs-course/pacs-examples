#include <iostream>    

using namespace std;
 
int main() {
  
  // here assumes an int occupies 32 bits

  int n;                       
  int mask = 1 << 31;                         // mask = 10000000 ... 00000000
  for (; ;) {                                 // enter an infinite loop
    cout << "\nEnter an integer: \n";         // input an integer
    cin >> n;                                 // input is assigned to n

    cout << "bit representation of " << n << " is: ";
    for (int i = 1; i <= 32; i++) {           // loop over all 32 bits
      char cc = (n & mask) ? '1': '0';        // take bit i from left to right
      cout << cc;                             // print bit i from left to right
      n <<= 1;                                // n = n << 1; left shift n 1 bit
      if (i%8 == 0 && i != 32) cout << ' ';   // print one space after each byte
    }
  }
}
