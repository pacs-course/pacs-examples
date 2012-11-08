// compare the efficiency of function tmeplate and passing function
// pointer

#include <iostream>      // for input and output 
#include <cstdlib>       // for qsort()
#include <ctime>         // for time()
#include <algorithm>     // for swap()
#include <complex>   

using namespace std;

template<class T> bool less2(T a, T b) { return a < b; } // a general template

template<class T> bool less2(complex<T> a, complex<T> b) { 
  return abs(a) < abs(b);                  // specialization, using <complex>
}  

struct point2d { 
  double x; double y; 
};

inline bool operator<(const point2d& p, const point2d& q) {
  return p.x < q.x;
}

template<> bool less2(point2d a, point2d b) { 
  return a.x < b.x;    
}  


template<class T> void sort2(T* v, int n) {
  // const unsigned long n = v.size();
  for (int gap = n/2; gap > 0; gap /= 2)
    for (int i = gap; i < n; i++)
      for (int j = i - gap; j >= 0; j -= gap) {
 
        // use the overloaded < operator
        if (v[j+gap] < v[j]) swap(v[j], v[j+gap]); 

        // use the overloaded less2 operator
        // if (less2(v[j+gap], v[j])) swap(v[j], v[j+gap]); 

        // use the overloaded operator < and compare directly
        /* 
        if (v[j+gap] < v[j]) {
          T temp = v[j];      
          v[j] = v[j+gap];      
          v[j+gap] = temp;
        }
        */

      }
}


typedef int (*CMF)(const void*, const void*);
void shsort(void* b, size_t n, size_t sz, CMF cmp) {
  // sort "n" elements of array "b" in increasing 
  // order defined by comparison function "cmp". 
  // Each element of "b" is of size "sz" (in bytes).

  for (size_t gap = n/2; gap > 0; gap /= 2)
    for (size_t i = gap; i < n; i++)
      for (int j = i - gap; j >= 0; j -= gap) {
        char* base = static_cast<char*>(b);
        char* pj = base + j*sz;
        char* pjpg = base + (j + gap)*sz;
        if (cmp(pj, pjpg) > 0) {       // swap b[j] and b[j+gap]
          for (size_t k = 0; k < sz; k++) {
            char temp = pj[k];      
            pj[k] = pjpg[k];
            pjpg[k] = temp;
          }
        }
      }
}


int cmpex(const void* r, const void* s) {
  double d = static_cast<const point2d*>(r)->x - 
             static_cast<const point2d*>(s)->x;
  if (d > 0) return 1;
  else if (d < 0) return - 1;
  else return 0;
}

int cmpey(const void* r, const void* s) {
  double d = static_cast<const point2d*>(r)->y - 
             static_cast<const point2d*>(s)->y;
  if (d > 0) return 1;
  else if (d < 0) return - 1;
  else return 0;
}


int main() {
  const int n = 10000;
  const int m = 10;
  point2d a[n];
  for (int i = 0; i < n; i++) { 
    a[i].x = i*(5 - i); a[i].y = 5/(i+2.3) - 1; 
  }


  time_t tm0 = time(0);
  for (int j = 0; j < m; j++) shsort(a, n, sizeof(point2d), cmpey);
  time_t tm1 = time(0);
  cout << "time in shsort = " << difftime(tm1, tm0) << '\n'; 

  for (int j = 0; j < m; j++) sort2(a,n);
  time_t tm2 = time(0);
  cout << "time in sort = " << difftime(tm2, tm1) << '\n'; 

  //  for (int i = 0; i < n; i++) 
  //  cout << a[i].x << "  " << a[i].y << '\n';

}
