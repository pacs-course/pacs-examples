
#include <cmath>
#include <iostream>

struct point2d {
  double x;
  double y;
  point2d() { x = 0; y = 0; }                       // default constructor
  point2d(double r) { x = r; y = 0; }               // constructor
  point2d(double r, double s) { x = r; y = s; }     // constructor

  void move(double r, double s) { x = r; y = s; }   // move to position (r,s)
  void draw() const { /* draw the point ...*/ }     // a const member 
  friend double norm(point2d p) {                   // a friend
    return sqrt(p.x*p.x + p.y*p.y);                 // distance to origin
  }
};

class triangle {
  point2d* vertices;
public:
  triangle(point2d, point2d, point2d);         // constructor
  triangle(const triangle&);                   // copy constructor
  triangle& operator=(const triangle&);         //copy assignment 
  ~triangle() { 
     std::cout << "deleting space for triangle\n";
     delete[] vertices; 
   }                                           // destructor
  double area() const;                         // a function member 
};

double triangle::area() const {
 return 0.5*std::abs(
   (vertices[1].x - vertices[0].x)*(vertices[2].y - vertices[0].y)
   - (vertices[1].y - vertices[0].y)*(vertices[2].x - vertices[0].x));
}

triangle::triangle(point2d v0, point2d v1, point2d v2) {
  std::cout << "allocating space for triangle\n";
  vertices = new point2d [3];
  vertices[0] = v0; 
  vertices[1] = v1; 
  vertices[2] = v2;
}

void f() {
  point2d x(1.0,2.0);
  point2d y(3.0);
  point2d z;
  point2d z2(7.0);
  triangle t1(x,y,z);
  triangle t2(x,y,z2);
}

void g(point2d x, point2d y, point2d z) {
  triangle t1(x,y,z);
  triangle t2 = t1;               // copy initialization, trouble here
  triangle t3(x,y,z);
  t3 = t1;                        // copy assignment, trouble here
}

triangle::triangle(const triangle & t) {              // copy constructor
  std::cout << "allocating space for triangle\n";
  vertices = new point2d [3];
  for (int i = 0; i < 3; i++) vertices[i] = t.vertices[i];
}

triangle& triangle::operator=(const triangle& t) {    // copy assignment
  if (this != &t)                // beware of self-assignment like t = t;
    for (int i = 0; i < 3; i++) vertices[i] = t.vertices[i];
  return *this;
}

class triple {                                  // a triple of numbers
  float* data;
public:
  triple(float a, float b, float c);            // constructor 
  ~triple() { delete[] data; }                  // destructor, also defined here
  triple(const triple& t);                      // copy constructor
  triple& operator=(const triple& t);           // copy assignment 
  
  friend triple add(const triple&, const triple&);      // add is a friend
  void print() {                                        // print out members
    std::cout << data[0] << "  " << data[1] << "  " << data[2] << std::endl;
  } 
};

inline triple::triple(float a, float b, float c) {
  data = new float [3];
  data[0] = a; data[1] = b; data[2] = c;
}

inline triple::triple(const triple& t) {
  data = new float [3];
  for (int i = 0; i < 3; i++) data[i] = t.data[i];
}

inline triple& triple::operator=(const triple & t) {
  if (this != &t) 
    for (int i = 0; i < 3; i++) data[i] = t.data[i];
  return *this;
}

inline triple add(const triple& t, const triple& s) {
  return triple(t.data[0] + s.data[0], t.data[1] + s.data[1], 
                t.data[2] + s.data[2]);
}

class duple {                                  // a duple of n numbers
  float* data;
  int size;
public:
  duple(int n, float* a) {                     // constructor 
    size = n;
    data = new float [size];
    for (int i = 0; i < size; i++) data[i] = a[i];
  }
  ~duple() { delete[] data; }                  // destructor, also defined here
  duple(const duple& t);                       // copy constructor
  duple& operator=(const duple& t);            // copy assignment 

  friend duple add(const duple& t, const duple& s);
  void print() {
   for (int i = 0; i < size; i++) std::cout << data[i] << "  ";
   std::cout << std::endl;
  }
};

duple& duple::operator=(const duple& t) {           // copy assignment 
  if (this != &t) {
    if (size != t.size) {
      delete[] data;
      data = new float [size = t.size];
    }
    for (int i = 0; i < size; i++) data[i] = t.data[i];
  }
  return *this;
}


int main() {
   float xx[] = { 5, 6, 7};
   float* zz = new float [10]; 
   for (int i = 0; i < 10; i++) zz[i] = i + 1;

   // test duple
   duple d1(3,xx);
   d1.print();
   duple d2(5,zz);
   d2.print();
   d1 = d2;                                   // d1 is re-sized 
   d1.print();
   

   // test triple
   triple aaa(5,6,7);
   aaa.print();
   triple bbb(10,20,30);
   bbb.print();
   triple ccc = aaa;                         // copy construnction
   ccc.print();
   triple ddd = add(aaa,bbb);                // copy construnction
   ddd.print();
   ccc = add(aaa,ddd);                       // copy assignment
   ccc = ccc;
   ccc.print();


   // test point2d
   point2d p;                        // p = (0,0) by the default constructor
   point2d p2(5);                    // p2 = (5,0) by the second constructor  
   point2d p3(0,5);                  // p3 = (0,5) by the third constructor  
   point2d p4 = p3;                  // p4 = (5,6.7), copy construction
   p4 = p2;                          // p4 = (5,0), copy assignment

   p4.move(23, -3.5);                // p4 = (23, -3.5), call a member function
   p4.draw();                        // draw p4
   double d = norm(p4);              // distance from p4 to origin

   triangle t1(p, p2, p3);
   std::cout << "area = " << t1.area() <<'\n';

   g(p, p2, p3);
}
