#include <iostream> 

class B {
  double* pd;
public:
  B() {
    pd = new double [20];
    std::cout << "20 doubles allocated\n";
  }
  virtual ~B() {
    delete[] pd;
    std::cout << "20 doubles deleted\n";
  }
};

class D: public B {
  int* pi;
public:
  D(): B() {
    pi = new int [1000];
    std::cout << "1000 ints allocated\n";
  }
  ~D() {
    delete pi;
    std::cout << "1000 ints deleted\n";
  }
};

int main() {
  B* p = new D;
  delete p;
}
