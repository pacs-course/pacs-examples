#include <iostream>   
#include <cfloat>   

int main() {

  using namespace std;

  double x = DBL_MAX;                     // x = biggest double precision number
  double epsilon = DBL_EPSILON;           // epsilon = double epsilon
  double zero = 0.0;
  double y = 100.2;

  double z0 = x + x;
  double z1 = x * 2;
  double z2 = epsilon/9;
  double z3 = y/zero;
  double z4 = zero/zero;
  double z5 = z3 - z3;
  double z6 = x + y;

  cout << "outputing results:\n";     
  cout << z0 << "\n";     
  cout << z1 << "\n";    
  cout << z2 << "\n";   
  cout << z3 << "\n";  
  cout << z4 << "\n"; 
  cout << z5 << "\n";
  cout << z6 << "\n"; 
  cout << 1 + z2 << "\n"; 


  bool b = 5e37 < 5e37 + 0.01;
  cout << "b = " << b << "\n"; 

  double sum;
  int i;
  for (i = 0, sum = 0; i < 100; sum += i, i++) ;
  cout  << "sum = " << sum << '\n';


  // this will enter an infinity loop due to roundoff errors
  // in representing float numbers.
  double summ = 0;
  for (double cx = 0.0; cx != 5.5; cx += 0.1)  summ += cx;
}
