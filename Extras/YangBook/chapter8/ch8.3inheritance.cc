#include <iostream> 

class BB {
private:
  int i;
protected:
  float f;
public:
  double d;

  BB(int a, float b, double c): i(a), f(b), d(c) { }
  void print() {
    std::cout << "int = " << i << ", float =  " << f << ", double =  " 
              << d << "." << std::endl;
  }
  void g1(BB& x) { x.f = 6; }
};

class X: protected BB {
protected:
  short s;
public:
  BB::d;         // change status of BB::d
  X(int a, float b, double c): BB(a,b,c) { }

  //  void g2(BB& x) { x.f = 6; }

  void g3(X& x) { x.f = 6; }

  void pt(X& x) { 
    x.f = 6.6;

    BB* b = &x;
    b->print();
  }
};

class Y: public X {
public:
  Y(int a, float b, double c): X(a,b,c) { }
  //  void Yg(Y& y) { s = y.s; f = y.f; }
  //  friend void Yf(Y& y, X& x) { x.s = 6; x.f = 6.6; y.f = 6.6; }
};

class B3 {
public:
  double d;
};

class Z: private B3 {               // private derivation
private:
  double d;                        // this d hides B::d
public:
  void f(Z& z) {
    z.d = 3.1415926;               // Z::d instead of B::d
    z.B3::d = 2.718;                // z's member B::d
  }
};

int main() {

  // inheritance
  X x(5,5.5,5.55);
  x.d = 6.67;
  x.pt(x);

  // BB* b = &x;
  // b->print();
}
