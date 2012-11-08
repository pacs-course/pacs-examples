#include <iostream> 
#include <complex>
#include <vector>
#include <cstring>
#include <algorithm>

using namespace std;

// a template for comparing
template<class T> bool compare(T a, T b) { return a < b; }

// a partial specialization for comparing
template<class T> bool compare(complex<T> a, complex<T> b) { 
  return abs(a) < abs(b);
}

// a complete specialization for comparing
template<> bool compare(const char* a, const char* b) { 
  return strcmp(a, b) < 0;
}

// another complete specialization for comparing
template<> bool compare(complex<double> a, complex<double> b) { 
  return a.real() < b.real();
  // return abs(a) < abs(b); // another possibility
}

// a template for sorting based on shell sort
template<class T> void sort(vector<T> & v) {
  typedef typename vector<T>::size_type size_t;
  size_t n = v.size();
  for (size_t gap = n/2; gap > 0; gap /= 2)
    for (size_t i = gap; i < n; i++)
      //      for (size_t j = i - gap; j >= 0; j -= gap)// ERRORE
      if(i>=gap){
	for (size_t j = i - gap+1; j > 0; j= (j>=gap? j-gap:0) )
	  if (compare(v[j+gap-1], v[j-1])) std::swap(v[j+gap-1],v[j-1]);
      }
}


int main(){
  int n = 3;
  int m = 10;
  vector<const char*> v1(n) ;
  v1[0] = "hi";
  v1[2] = "hill";
  v1[1] = "hello";
  
  vector< complex<double> > v2(m) ;
  for (int i=0; i <m;i++) v2[i] = complex<double>(std::sin((6.28/m)*i), 3-i);

  for (int i=0; i <n;i++) { cout << v1[i]  << " "; }
  cout<<endl;
  for (int i=0; i <m;i++) { cout << v2[i]  << " "; }
  cout << "\n"; 

  sort(v1);
  sort(v2);
  for (int i = 0; i < n; i++ ) cout << v1[i] << " ";
  cout<<endl;
  for (int i = 0; i < m; i++ ) cout << v2[i] << " ";

}  // end of main()
