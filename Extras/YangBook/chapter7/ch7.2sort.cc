#include <iostream> 
#include <complex>
#include <vector>
#include <cstring>

using namespace std;

// a template for comparing
template<class T> bool less2(T a, T b) { return a < b; }

// a partial specialization for comparing
template<class T> bool less2(complex<T> a, complex<T> b) { 
  return abs(a) < abs(b);
}

// a complete specialization for comparing
template<> bool less2(const char* a, const char* b) { 
  return std::strcmp(a, b) < 0;
}

// another complete specialization for comparing
template<> bool less2(complex<double> a, complex<double> b) { 
  return b.real() < a.real();
}

// a template for sorting based on shell sort
template<class T> void sort(vector<T> & v) {
  typedef typename vector<T>::size_type sz;
  sz n = v.size();
  for (sz gap = n/2; gap > 0; gap /= 2)
    for (sz i = gap; i < n; i++)
      for (sz j = i - gap; j >= 0; j -= gap)
        if (less2(v[j+gap], v[j])) {
          T temp = v[j];
          v[j] = v[j+gap];
          v[j+gap] = temp;
        }
}


int main(){
  int n = 3;

  vector<const char*> v1(n) ;
  v1[0] = "hi";
  v1[1] = "hill";
  v1[2] = "hello";

  vector< complex<double> > v2(n) ;
  for (int i=0; i <n;i++) v2[i] = complex<double>(i*i - 1, 3-i);

  for (int i=0; i <n;i++) { cout << v1[i]  << " "; }
  for (int i=0; i <n;i++) { cout << v2[i]  << " "; }
  cout << "\n"; 

  sort(v1);
  sort(v2);
  for (int i = 0; i < n; i++ ) cout << v1[i] << " ";
  for (int i = 0; i < n; i++ ) cout << v2[i] << " ";

}  // end of main()
