#include <cmath>
#include <iostream>
#include <algorithm>

const double lambda = - 50;
const int n = 100;

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
  return x*(1 - exp(t))/(1 + exp(t));
}

double dfdx(double t, double x) {    // derivative of f wrt x
  return (1 - exp(t))/(1 + exp(t));
}

double exact(double t) {
  return 12*exp(t)/pow(1 + exp(t), 2);
}

double f2(double t, double x) {
  return lambda*x;
}

double df2dx(double t, double x) {    // derivative of f2 wrt x
  return lambda;
}

double exact2(double t) {
  return exp(lambda*t);
}


int main() {

  double norm = 0;
  double normimpt = 0;
  double normpc = 0;
  double normrk2 = 0;
  double normrk4 = 0;

{  // example 1 
  ode exmp(0, 3, 2, f); 
  double* soln = exmp.euler(n);
  double* solnimpt = exmp.eulerimpt(n, dfdx, 1e-10, 50);
  double* solnpc = exmp.eulerpc(n);
  double* solnrk2 = exmp.rk2(n);
  double* solnrk4 = exmp.rk4(n);

  double h = 2.0/n;
  for (int i = 1; i <= n; i++)   {
    norm = std::max(norm, fabs(exact(i*h) - soln[i]));
    normimpt = std::max(normimpt, std::abs(exact(i*h) - solnimpt[i]));
    normpc = std::max(normpc, std::abs(exact(i*h) - solnpc[i]));
    normrk2 = std::max(normrk2, std::abs(exact(i*h) - solnrk2[i]));
    normrk4 = std::max(normrk4, std::abs(exact(i*h) - solnrk4[i]));
    // std::cout << i << "   " << exact(i*h)  << "   " << soln[i] << '\n';
  }

  delete[] soln;
  delete[] solnpc;
  delete[] solnrk2;
  delete[] solnrk4;

} // end example 1


/*
{  // example 2 
  ode exmp(0, 1, 2, f2); 
  double* soln = exmp.euler(n);
  double* solnimpt = exmp.eulerimpt(n, df2dx, 1e-15, 500);
  double* solnpc = exmp.eulerpc(n);
  double* solnrk2 = exmp.rk2(n);
  double* solnrk4 = exmp.rk4(n);

  double h = 2.0/n;
  for (int i = 1; i <= n; i++)   {
    norm = max(norm, fabs(exact2(i*h) - soln[i]));
    normimpt = max(normimpt, fabs(exact2(i*h) - solnimpt[i]));
    normpc = max(normpc, fabs(exact2(i*h) - solnpc[i]));
    normrk2 = max(normrk2, fabs(exact2(i*h) - solnrk2[i]));
    normrk4 = max(normrk4, fabs(exact2(i*h) - solnrk4[i]));
  }

  delete[] soln;
  delete[] solnpc;
  delete[] solnrk2;
  delete[] solnrk4;
} // end example 2
*/


  std::cout <<"max norm of error by euler's method = " << norm << '\n'; 
  std::cout <<"max norm of error by implicit euler's method = "<<normimpt<<'\n'; 
  std::cout <<"max norm of error by predictor-corrector euler's method = " 
            << normpc << '\n'; 
  std::cout << "max norm of error by 2nd order Runge-Kutta method = " 
            << normrk2 << '\n'; 
  std::cout << "max norm of error by 4nd order Runge-Kutta method = " 
            << normrk4 << '\n'; 

}
