#include <cmath>
#include <iostream>

class Vec {
private:
  int size;
  double* entry;
  static Vec defaultVec;                  // variable for holding default vector
public:
  Vec(int sz = 0, double* et = 0);
  Vec(const Vec&);
  Vec& operator=(const Vec&);
  static void setDefault(int, double*);   // function for setting default vector

  void print() {
    for (int i = 0; i < size; i++) std::cout  << entry[i] << " ";
    std::cout  << std::endl;
  }
};

Vec::Vec(int sz, double* et) {
  size = (sz ? sz: defaultVec.size);      // if sz = 0 use default size
  entry = new double [size];
  if (et == 0) {                          // if et is null pointer, use default
    for (int i = 0; i < size; i++)
      entry[i] = defaultVec.entry[i];
  } else {
    for (int i = 0; i < size; i++) entry[i] = et[i];
  }
}

inline Vec::Vec(const Vec& v) {
  entry = new double [size = v.size];
  for (int i = 0; i < size; i++) entry[i] = v.entry[i];
}

Vec& Vec::operator=(const Vec& v) {
  if (this != &v) {
    if (size != v.size){
      delete[] entry;
      entry = new double [size = v.size];
    }
    for (int i = 0; i < size; i++) entry[i] = v.entry[i];
  }
  return *this;
}

double et[] = {5, 6, 7, 8, 9};
Vec Vec::defaultVec(5,et);           // defaultVec initialized by et of size 5

void Vec::setDefault(int sz, double* et) {       // reset default vector
  Vec::defaultVec = Vec(sz,et);      // note class qualification Vec::defaultVec
}

void f() {
  Vec a;                             // a is default vector of 5
  Vec b(3);                          // b is default vector of 3
  a.print();
  b.print();

  double* ey = new double [8];
  for (int i = 0; i < 8; i++ ) ey[i] = 1;
  Vec::setDefault(8,ey);             // reset defaultVec to be unit of size 8

  Vec a2;                            // a2 is new default unit vector of size 8
  Vec b2(5);                         // b2 is new default unit vector of size 5
  a2.print();
  b2.print();

  double* enb = new double [9];
  for (int i = 0; i < 9; i++ ) enb[i] = 8*i*i + 100;  
  Vec c(9,enb);                      // c is a vector constructed from enb.
  c.print();
}


int main() {
  f();
}
