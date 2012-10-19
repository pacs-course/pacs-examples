#include <cmath>
#include <iostream>

class Vec {
  double* en;
  int size;
public:
  Vec(int, double* = 0);
  double f();
};

Vec::Vec(int s, double* d) {
  en = new double [size = s];
  if (d) {                                  // if d is not the null pointer
    for (int i = 0; i < size; i++) en[i] = d[i];
  } else {                                  // if d is null pointer, use default
    for (int i = 0; i < size; i++) en[i] = 0;
  }
}

double Vec::f() {
  Vec** tm = new Vec* [size];               // allocate space for the matrix
  for (int i = 0; i < size; i++) {
    tm[i] = new Vec(i+1);                   // construct zero vector of size i+1
  }

  for (int i = 0; i < size; i++) 
    for (int j = 0; j <= i; j++)            // assign some values to entries
      tm[i]->en[j] = 1.0/(i + j + 1);

  double sum = 0;
  for (int i = 0; i < size; i++) 
    for (int j = 0; j <= i; j++)            // assign some values to entries
      sum += tm[i]->en[j];

  for (int i = 0; i < size; i++ ) delete tm[i];    // deallocate space after use
  delete[] tm;

  return sum;
}

int main() {
 Vec a(7);
 std::cout << "sum = " << a.f() << '\n';
}
