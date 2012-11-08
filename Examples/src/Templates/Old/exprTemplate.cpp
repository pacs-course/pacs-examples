// expression template

#include <vector>
#include <iostream>

// construct an object to hold left operand, operation, 
// and right operand. Defer intermediate evaluations
// and thus avoid temporary vector obect and loops.

template<class LeftOpd, class Op, class RightOpd>
struct LOR {
  LeftOpd fod;      // store left operand
  RightOpd rod;     // store right operand

  LOR(LeftOpd p, RightOpd r): fod(p), rod(r) { }

  double operator[](int i) { 
    return Op::apply(fod[i], rod[i]); 
  }
};

// define a simple vector class
// as a view of a C array
class Vtr {
  double* vr;
  int lenth;
public:
  Vtr(double* d, int n) { vr = d; lenth = n; }

  // assign an expression to a vector and
  // do the evaluation
  template<class LeftOpd, class Op, class RightOpd>
  void operator=(LOR<LeftOpd, Op, RightOpd> exprn) {
    for (int i = 0; i < lenth; i++) vr[i] = exprn[i];
  }

  double operator[](int i) const { return vr[i]; }
};

// define multiply operation on elements on vectors
struct Multiply {
  static double apply(double a, double b) {
    return a*b;
  }
};

// overload operator *: defer evaluation 
template<class LeftOpd> 
LOR<LeftOpd, Multiply, Vtr> operator*(LeftOpd a, Vtr b) {
  return LOR<LeftOpd, Multiply, Vtr>(a,b);
}

int main() {
  double a[] = { 1, 2, 3, 4, 5};
  double b[] = { 1, 2, 3, 4, 5};
  double c[] = { 1, 2, 3, 4, 5};
  double d[4];

  Vtr X(a,5), Y(b,5), Z(c,5), W(d,5);
  W = X * Y * Z;

  for (int i = 0; i< 5; i++) std::cout << W[i] << "  ";
  std::cout << '\n';
}
