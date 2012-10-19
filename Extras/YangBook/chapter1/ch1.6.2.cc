#include <iostream>    

using namespace std;

const int mxdigits = 999500;       // maximum number of digits in factorial

int main() {
  int n;                       
  cout << "Enter a positive integer: \n";  
  cin >> n;                   

  // digits of n! will be stored in array dgs backwards, ie, highest digit
  // will be stored in dgs[0], next highest in dgs[1], and so on.
  int dgs[mxdigits];            
  dgs[0] = 1;
  for (int i = 1; i < mxdigits; i++) dgs[i] = 0;
  for (int k = 2; k <= n; k++) {          // n! = 2*3*4....(n-1)*n
    int cary = 0;
    for (int j = 0; j < mxdigits; j++) {  // k * every digit of (k-1)! to get k!
      dgs[j] = k*dgs[j] + cary;
      cary = dgs[j]/10;
      dgs[j] -= 10*cary;
    }
  }

  // counting the number of digits in n!
  int count = mxdigits - 1;
  while (!dgs[count]) count--;
  if (count >= mxdigits -6) cout << "factorial overflew, adjust parameter.\n";

  // printing n!: lowest digit dgs[count] first, highest digit dgs[0] last. 
  cout << "There are " << count + 1 << " digits in " << n << "!. They are:\n";
  for (int i = count; i >= 0; i--) cout << dgs[i]; 
  cout << '\n';

  // compare to direct multiply:
  //
  // this simple code can only compute n! correctly for n = 1 up to n = 12.
  // When n = 13, a computer with sizeof(int) = 4 computes 13! as
  // 1932053504 while the correct value is 13! = 6227020800. And it 
  // computes 20! as  -2102132736 and 40! as 0. 
  // In Excercise  , a technique is introduced that computes n! correctly
  // for much larger n, for example n = 3000. Notice that 3000! has 9131 
  // digits.
  int fac = 1;
  for (int i = 2; i <= n; i++) fac *= i; 
  cout << n << "! is (in direct multiply): " << fac << "\n";

}
