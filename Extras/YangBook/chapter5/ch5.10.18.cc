// Ms Lee's account

#include <cmath>
#include <iostream>
#include <algorithm>

class ode {
  double tini;                             // initial time
  double ison;                             // initial solution
  double tend;                             // end time

  typedef double (*ptrf2v)(double, double);  // fcn of 2 variables
  ptrf2v sfn;                                // source function
public:
  ode(double t0, double x0, double T, ptrf2v f) {
    tini = t0; ison = x0; tend = T; sfn = f;
  } 
  double* euler(int n) const;              // euler' method with n subintervals
  double* eulerimpt(int n, ptrf2v, double eps, int maxit) const;          
                                           // implicit euler's method
  double* eulerpc(int n) const;            // predictor-corrector Euler method 
  double* rk2(int n) const;                // second order Runge-Kutta method 
  double* rk4(int n) const;                // fourth order Runge-Kutta method 
};


double* ode::euler(int n) const {
  double* x = new double [n + 1];
  double h = (tend - tini)/n;
  x[0] = ison;
  for (int i = 0; i < n; i++) x[i+1] = x[i] + h*sfn(tini + i*h, x[i]);
  return x;
}

double* ode::eulerimpt(int n, ptrf2v dfdx, double eps, int maxit) const {
  double* x = new double [n + 1];
  double h = (tend - tini)/n;
  x[0] = ison;
  for (int i = 0; i < n; i++) {
    double tc = tini + (i+1)*h;
    double yp = x[i];
    int j = 0;
    for ( ; j < maxit; j++) {
      double yc = yp - (yp - x[i] - h*sfn(tc, yp))/(1 - h*dfdx(tc,yp)); 
      if (fabs(yc - yp) <= eps || fabs(yc - x[i] - h*sfn(tc, yc)) <= eps) {
        x[i + 1] = yc;
        break;
      } else {
        yp = yc;
      }
    }
    if (j == maxit) {
      std::cout << "newton method did not converge within given number\n"
           << "of iterations in function: eulerimpt()\n";
      exit(1);
    }   
  }
  return x;
}

double* ode::eulerpc(int n) const {
  double* x = new double [n + 1];
  double h = (tend - tini)/n;
  x[0] = ison;
  for (int i = 0; i < n; i++) {
    x[i+1] = x[i] + h*sfn(tini + i*h, x[i]);          // predictor
    x[i+1] = x[i] + h*sfn(tini + (i+1)*h, x[i+1]);    // corrector
  }
  return x;
}

double* ode::rk2(int n) const {
  double* x = new double [n + 1];
  double h = (tend - tini)/n;
  x[0] = ison;
  for (int i = 0; i < n; i++) {
    double tp = tini + i*h;
    double f = h*sfn(tp, x[i]);
    x[i+1] = x[i] + 0.5*(f + h*sfn(tp + h, x[i] + f));
  }
  return x;
}

double* ode::rk4(int n) const {
  double* x = new double [n + 1];
  double h = (tend - tini)/n;
  x[0] = ison;
  for (int i = 0; i < n; i++) {
    double tp = tini + i*h;
    double tp2 = tp + 0.5*h;
    double f1 = h*sfn(tp, x[i]);
    double f2 = h*sfn(tp2, x[i] + 0.5*f1);
    double f3 = h*sfn(tp2, x[i] + 0.5*f2);
    x[i+1] = x[i] + (f1 + 2*(f2+f3) + h*sfn(tp + h, x[i] + f3))/6;
  }
  return x;
}

double f(double t, double x) {
  if (t < 10) return 0.05*x;
//  else return 0.05*x - 1000*exp(0.03*(t-10));
  else return 0.05*x - 1000;
}

double dfdx(double t, double x) {    // derivative of f wrt x
  return 0.05;
}

int main() {

  // Ms Lee's account

  const int n = 100000;
  ode exmp(0, 10000, 50, f); 
  double* soln = exmp.euler(n);

  double h = 50.0/n;
  for (int i = 1; i <= n; i++)   {
    if (soln[i] <= 0) {
      std::cout << "i = " << i << '\n';
      std::cout << "h = " << h << '\n';
      std::cout << "time = " << ( i + i - 1)*h/2 << '\n';
      exit(1);
    }  
  }

}
