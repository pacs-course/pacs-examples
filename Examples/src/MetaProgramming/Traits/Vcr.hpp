#include <iostream> 
#include <cmath>
#include <complex>
#include <cstdlib> // for exit()

using namespace std;

template<class T> class Vcr {       
  int lenth;                       // number of entries in vector
  T* vr;                           // entries of the vector
public: 
  Vcr(int, T*);                    // constructor
  Vcr(const Vcr&);                 // copy constructor
  ~Vcr(){ delete[] vr; }           // destructor

  int size() { return lenth; }
  T& operator[](int i) const { return vr[i]; }
  Vcr& operator=(const Vcr&);      // overload = 
  Vcr& operator+=(const Vcr&);     // v += v2
  Vcr& operator-=(const Vcr&);     // v -= v2
  T maxnorm () const;              // maximum norm
  T twonorm() const;               // L-2 norm

  template<class S>
  friend S dot(const Vcr<S>&, const Vcr<S>&);  // dot product
};

// partial specialization
template<class T> class Vcr< complex<T> > {       
  int lenth;                       // number of entries in vector
  complex<T>* vr;                  // entries of the vector
public: 
  Vcr(int, complex<T>*);           // constructor
  Vcr(const Vcr&);                 // copy constructor
  ~Vcr(){ delete[] vr; }           // destructor

  int size() { return lenth; }
  complex<T>& operator[](int i) const { return vr[i]; }
  Vcr& operator=(const Vcr&);    
  Vcr& operator+=(const Vcr&);     // v += v2
  Vcr& operator-=(const Vcr&);     // v -= v2
  T maxnorm () const;              // maximum norm
  T twonorm() const;               // L-2 norm

  template<class S>
  friend complex<S> dot(const Vcr<complex<S> >&, 
                        const Vcr<complex<S> >&);      // dot product
};


// complete specialization
template<>
class Vcr< complex<double> > {       
  int lenth;                       // number of entries in vector
  complex<double>* vr;             // entries of the vector
public: 
  Vcr(int, complex<double>*);      // constructor
  Vcr(const Vcr&);                 // copy constructor
  ~Vcr(){ delete[] vr; }           // destructor
  int size() { return lenth; }
  complex<double>& operator[](int i) const {return vr[i];}
  Vcr& operator=(const Vcr&);    
  Vcr& operator+=(const Vcr&);     // v += v2
  Vcr& operator-=(const Vcr&);     // v -= v2
  double maxnorm() const;          // maximum norm
  double twonorm() const;          // L-2 norm

  friend complex<double> dot(const Vcr&, const Vcr&);    // dot product
};

//*** report error and exit with return value 1.
inline void error(char const * astring, ostream& s = cout) {
  s << astring << " Program exited." << endl;
  exit(1);
};

template<class T> 
Vcr<T>::Vcr(int n, T* abd) {
  vr = new T [lenth =n]; 
  for (int i = 0; i < lenth; i++)  vr[i]= *(abd +i);
}

template<class T> 
Vcr<T>::Vcr(const Vcr & vec) {
  vr = new T [lenth = vec.lenth]; 
  for (int i = 0; i < lenth; i++)  vr[i] = vec[i]; 
}

template<class T> 
Vcr<T>& Vcr<T>::operator=(const Vcr& vec) {
  if (this != &vec) {
    if (lenth != vec.lenth ) error("bad vector sizes");
    for (int i = 0; i < lenth; i++) vr[i] = vec[i];
  }
  return *this;
}

template<class T> 
Vcr<T> & Vcr<T>::operator+=(const Vcr& vec) {
  if (lenth != vec.lenth ) error("bad vector sizes");
  for (int i = 0; i < lenth; i++) vr[i] += vec[i];
  return *this;
}

template<class T> 
Vcr<T> & Vcr<T>::operator-=(const Vcr& vec) {
  if (lenth != vec.lenth ) error("bad vector sizes");
  for (int i = 0; i < lenth; i++) vr[i] -= vec[i];
  return *this;
}

template<class T> T Vcr<T>::maxnorm() const {
  T nm = abs(vr[0]);
  for (int i = 1; i < lenth; i++) {
    T avi = abs(vr[i]);
    if (nm < avi) nm = avi;
  }
  return nm;
}

template<class T> 
T dot(const Vcr<T> & v1, const Vcr<T> & v2) {
  if (v1.lenth != v2.lenth ) error("bad vector sizes");
  T tm = v1[0]*v2[0];
  for (int i = 1; i < v1.lenth; i++) tm += v1[i]*v2[i];
  return tm;
}

template<class T> 
Vcr< complex<T> >::Vcr(int n, complex<T>* abd) {
  vr = new complex<T> [lenth = n]; 
  for (int i = 0; i < lenth; i++)  vr[i]= *(abd +i);
}

template<class T> 
Vcr< complex<T> >::Vcr(const Vcr & vec) {
  vr = new complex<T> [lenth = vec.lenth]; 
  for (int i = 0; i < lenth; i++)  vr[i] = vec[i]; 
}

template<class T> 
Vcr< complex<T> >& 
Vcr< complex<T> >::operator=(const Vcr& vec) {
  if (this != &vec) {
    if (lenth != vec.lenth ) error("bad vector sizes");
    for (int i = 0; i < lenth; i++) vr[i] = vec[i];
  }
  return *this;
}

template<class T> 
Vcr< complex<T> >& 
Vcr< complex<T> >::operator+=(const Vcr& vec) {
  if (lenth != vec.lenth ) error("bad vector sizes");
  for (int i = 0; i < lenth; i++) vr[i] += vec[i];
  return *this;
}

template<class T> 
Vcr< complex<T> >& 
Vcr< complex<T> >::operator-=(const Vcr& vec) {
  if (lenth != vec.lenth ) error("bad vector sizes");
  for (int i = 0; i < lenth; i++) vr[i] -= vec[i];
  return *this;
}

template<class T> T Vcr< complex<T> >::maxnorm() const {
  T nm = abs(vr[0]);
  for (int i = 1; i < lenth; i++) {
    T avi = abs(vr[i]);
    if (nm < avi) nm = avi;
  }
  return nm;
}

template<class T> 
complex<T> dot(const Vcr< complex<T> >& v1, 
               const Vcr< complex<T> >& v2){
  if (v1.lenth != v2.lenth ) error("bad vector sizes");
  complex<T> tm = v1[0] * conj(v2[0]);
  for (int i = 1; i < v1.lenth; i++) tm += v1[i]*conj(v2[i]);
  return tm;
}

Vcr< complex<double> >::Vcr(int n, complex<double>* abd) {
  vr = new complex<double> [lenth =n]; 
  for (int i = 0; i < lenth; i++)  vr[i]= *(abd + i);
}

Vcr< complex<double> >::Vcr(const Vcr & vec) {
  vr = new complex<double> [lenth = vec.lenth]; 
  for (int i = 0; i < lenth; i++)  vr[i] = vec[i]; 
}

Vcr< complex<double> >& 
Vcr< complex<double> >::operator-=(const Vcr& vec) {
  if (lenth != vec.lenth ) error("bad vector sizes");
  for (int i = 0; i < lenth; i++) vr[i] -= vec[i];
  return *this;
}

Vcr< complex<double> >& 
Vcr< complex<double> >::operator+=(const Vcr& vec) {
  if (lenth != vec.lenth ) error("bad vector sizes");
  for (int i = 0; i < lenth; i++) vr[i] += vec[i];
  return *this;
}


double Vcr< complex<double> >::maxnorm() const {
  double nm = abs(vr[0]);
  for (int i = 1; i < lenth; i++) {
    double avi = abs(vr[i]);
    if (nm < avi) nm = avi;
  }
  return nm;
}

complex<double> dot(const Vcr<complex<double> >& v1,
                    const Vcr<complex<double> >& v2) {
  if (v1.lenth != v2.lenth ) error("bad vector sizes");
  complex<double> tm = v1[0]*conj(v2[0]);   // conjugation
  for (int i = 1; i < v1.lenth; i++) tm += v1[i]*conj(v2[i]);
  return tm;
}


void f(){
  int n = 1;
  complex<double>* aa = new complex<double> [n];
  for (int j = 0; j < n; j++) aa[j] = complex<double>(0.0, j+1.0);
  Vcr< complex<double> > v1(n, aa) ;                      // vector v1

  complex<double>* bb = new complex<double> [n];
  for (int j = 0; j < n; j++) bb[j] = complex<double>(2.0, 3.0 + j);
  Vcr< complex<double> > v2(n, bb);                           // vector v2

  cout << "v1= "     << v1[0]    << '\n';          // dot product
  cout << "v2= "     << v2[0]    << '\n';          // dot product

  cout << "dot = "     << dot(v1,v2)    << '\n';          // dot product
  cout << "maxnorm = " <<  v1.maxnorm() << '\n';          // max norm of v1
}  

int main(){

  f();

  int n = 3;

  complex<float>* aa = new complex<float> [n];
  complex<float >* bb = new complex<float> [n];
  complex<double>* cc = new complex<double> [n];
  complex<double>* dd = new complex<double> [n];

  float * a = new float [n];
  float * b = new float [n];

  for (int i=0; i <n;i++) {
    aa[i] = complex<float>(0,i);
    bb[i] = complex<float>(1,i);
    cc[i] = complex<double>(0,i);
    dd[i] = complex<double>(1,i);
    a[i] = i - 10;
    b[i] = i * i;
  }

  Vcr< complex<float> > v1(n, aa);
  Vcr< complex<float> > v2(n, bb);
  Vcr< complex<double> > v1d(n, cc);
  Vcr< complex<double> > v2d(n, dd);

  Vcr<float> v3(n, a);
  Vcr<float> v4(n, b);

  for (int i=0; i <n;i++) { cout << v1[i]  << "  "; }
  for (int i=0; i <n;i++) { cout << v2[i]  << "  "; }
  cout << " \n "; 
  for (int i=0; i <n;i++) { cout << v3[i]  << "  "; }
  for (int i=0; i <n;i++) { cout << v4[i]  << "  "; }

  cout << "\n"; 
  cout << "dot of v1 and v2 = " << dot(v1, v2) << "\n";
  cout << "dot of v1d and v2d = " << dot(v1d, v2d) << "\n";
  cout << "dot of v2 and v1 = " << dot(v2, v1) << "\n";
  cout << "dot of v3 and v4 = " << dot(v3, v4) << "\n";

  v1 += v2;
  v1d += v2d;
  v3 += v4;

  for (int i = 0; i < n; i++ ) cout << v1[i] << "  ";

  //cout << "maxnorm  of v2 = " << v2.maxnorm() << "\n";
  cout << "maxnorm  of v2d = " << v2d.maxnorm() << "\n";
  cout << "maxnorm  of v3 = " << v3.maxnorm() << "\n";
}  // end of main()
