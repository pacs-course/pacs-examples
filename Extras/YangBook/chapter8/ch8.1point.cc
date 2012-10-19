#include <cstdlib>
#include <iostream> 
#include <cmath>
#include <complex>
#include <vector>
#include <set>

class Pt {
private:
  double x;
public:
  Pt(double a = 0) {  x = a; }
  virtual void draw() { std::cout << x; } 
};

class Pt2d: public Pt {
private:
  double y;
public:
//  Pt2d(double a = 0, double b = 0): Pt(a), y(b) { }
  Pt2d(double a = 0, double b = 0): Pt(a) { y = b; }
  void draw() { 
    Pt::draw();
    std::cout << " " << y; 
  } 
};

class Pt3d: public Pt2d {
private:
  double z;
public:
  Pt3d(double a = 0, double b = 0, double c = 0): Pt2d(a,b) { z = c; }
  void draw() { 
    Pt2d::draw();
    std::cout << " " << z; 
  } 
};


void f(Pt p1, Pt2d p2) {
  Pt* p1pt = &p2;      // OK. Every Pt2d is a Pt
  //  Pt2d* p2pt = &p1;   // illegal.  Not every Pt is a Pt2d
 
  Pt2d* p2pt2 = static_cast<Pt2d*>(p1pt);  // OK, explicit type conversion
  p2pt2->draw();                           // x and y of p2 will be printed

  Pt2d* p2pt3 = static_cast<Pt2d*>(&p1);   // OK, explicit type conversion
  p2pt3->draw();                           // legal, but y may be garbage
}

void g(const std::vector<Pt*>& v) {        // use standard library vector<T>
  for (unsigned int i = 0; i < v.size(); i++) {
    v[i]->draw(); 
    std::cout << std::endl;
  }
}

void hf(std::set<Pt*>& s) {            // use standard library set<T>
  for (std::set<Pt*>::const_iterator p = s.begin(); p != s.end(); p++) {
    (*p)->draw(); 
    std::cout << std::endl;
  }
}


int main() {
  Pt a(5);
  Pt2d b(4,9);
  Pt3d c(7,7,7);

  f(a,b);

  std::vector<Pt*> v(3);
  v[0] = &a;
  v[1] = &b;
  v[2] = &c;
  g(v);
}
