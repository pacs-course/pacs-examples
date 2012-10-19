#include <iostream>     // include a library for input and output 
#include <cstdlib>      // include a library for atoi

using namespace std;

int main(int argc, char* argv[]) {

struct point2d {
  char nm;
  float x;
  float y;
};

point2d pt2 = { 'A', 3.14, -38 };
cout << "char = " << pt2.nm << "  float x = " << pt2.x << '\n';

point2d pt3 = pt2;
cout << "char = " << pt3.nm << "  float x = " << pt3.x << '\n';

union val {
  int i;
  double d;
  char c;
};

val x;
x.i = 5;
cout << "x.i = " << x.i << ", x.d = " << x.d << ", x.c = " << x.c << '\n';
x.d = 6.28;
cout << "x.i = " << x.i << ", x.d = " << x.d << ", x.c = " << x.c << '\n';
x.c = 'G';
cout << "x.i = " << x.i << ", x.d = " << x.d << ", x.c = " << x.c << '\n';

}
