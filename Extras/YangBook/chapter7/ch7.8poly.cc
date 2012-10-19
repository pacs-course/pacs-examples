#include <iostream> 
#include <cstdlib> 
#include <vector>   
#include <cmath>   

template<class T> 
T lagrange(const std::vector<T>& vx, const std::vector<T>& vy, T x) {
  int n = vx.size() - 1;
  T y = 0;
  for (int i = 0; i <= n; i++) {
    T temp = 1;
    for (int j = 0; j <= n; j++) 
      if (j != i) temp *= (x - vx[j])/(vx[i] - vx[j]);
    y += temp*vy[i];
  }
  return y;
}

template<class T> 
T newton(const std::vector<T>& vx, const std::vector<T>& vy, T x) {
  std::vector<T> b = vy;
  int n = vx.size() - 1;

  // find coefficients in Newton's form
  for (int j = 1; j <= n; j++) 
    for (int i = n; i >= j; i--) 
      b[i] = (b[i] - b[i-1])/(vx[i] - vx[i-j]);

  // evaluate interpolation polynomial at x
  T u = b[n];
  for (int i = n - 1; i >= 0; i--) u = b[i] + (x - vx[i])*u;
  return u;
}
int main() {
  std::cout.precision(20);
  const int n = 4;
  std::vector<float> px(n);
  std::vector<float> py(n);

  // px[0] = 5; px[1] = 7; px[2] = 6; px[3] = 6.6;
  // py[0] = 1; py[1] = -23; py[2] = -50; py[3] = -4;

  for (int i = 0; i < n; i++) {
    px[i] = 1 + i/4.0; py[i] = exp( px[i] );
  }

  for (int i = 0; i < n; i++) {
    std::cout << px[i]  << " " << py[i] << std::endl;
  }

  float x = 1.4;

  // use x = 6.2 will give big errors, it is too far from
  // interpolation nodes.
  // float x = 6.2;
  float aprxm = lagrange(px, py, x);
  std::cout << "aprm = " << aprxm << '\n';
  std::cout << "error = " << exp(x) - aprxm << '\n';

  float aprxm2 = newton(px, py, x);
  std::cout << "aprm = " << aprxm2 << '\n';
  std::cout << "error = " << exp(x) - aprxm2 << '\n';
}
