#include <iostream>
#include <cmath>
#include <cstdlib>

const double delta = 1.0e-8;
const double epsn  = 1.0e-9;

double bisctn0(double a, double b, double (*f)(double), 
	       double delta, double epsn);

double bisctn1(double a, double b, double (*f)(double),  double u,
	       double delta, double epsn);

double bisctn2(double a, double b, double (*f)(double),  double u,
	       double delta, double epsn, int maxit);

double bisectionr(double a, double b, double (*f)(double),  double u,
	          double delta, double epsn, int maxit, int& itern);

double bisection(double a, double b, double (*f)(double), 
	         double delta, double epsn, int maxit);

double fa(double x);
double fb(double x);
double fc(double x);
