#include <iostream>     // include a library for input and output 
#include <cstdlib>      // include a library for atoi

using namespace std;

int* tf() {
  int* local = new int;
  *local = 555;

  cout << " local = " <<  local << endl;
  cout << " *local = " <<  *local << endl;
  return local;
}

int main(int argc, char* argv[]) {

  int* relo = tf();
  cout << " relo = " <<  relo << endl;
  cout << " *relo = " <<  *relo << endl;
}
