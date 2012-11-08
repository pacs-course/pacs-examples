// template metaprogram

#include <iostream>

template<int N, class T>
class smallVtr {
  T vr[N];        // array of size N and type T
public:
  T& operator[](int i) { return vr[i]; }
};

template<int M> struct metaDot {

  template<int N, class T>
  static T f(smallVtr<N,T>& u, smallVtr<N,T>& v) {
    return u[M-1]*v[M-1] + metaDot<M - 1>::f(u,v);
  }

};

template<> struct metaDot<1> {

  template<int N, class T>
  static T f(smallVtr<N,T>& u, smallVtr<N,T>& v) {
    return u[0]*v[0];
  }

};

template<int N, class T>
inline T dot(smallVtr<N,T>& u, smallVtr<N,T>& v) {
  return metaDot<N>::f(u,v);
}

int main() {
  smallVtr<4, float> u, v;
  for (int i = 0; i < 4; i++) {
    u[i] = i + 1;
    v[i] = (i + 2)/3.0;
  }

  double d = dot(u,v);

  std::cout << " dot = " << d << '\n';
}
