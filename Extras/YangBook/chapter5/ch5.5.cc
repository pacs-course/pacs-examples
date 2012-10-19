#include <cmath>
#include <iostream>

class integral {    // members by default are private
  double lower;     // lower integral bound, a private data member 
  double upper;     // upper integral bound, private 

  //  double (*integrand)(double);
  typedef double (*pfn)(double);  // define pfn to be type of a function pointer
  pfn integrand;    // integrand takes a double and returns a double

  mutable double value;                   // store value from trapezoidal rule
  mutable bool value_valid;               // value valid or not
public:             // public members
  //  integral(double a, double b, double(*f)(double)){      // a constructor 
  integral(double a, double b, pfn f){      // a constructor 
    lower = a;
    upper = b; 
    integrand = f;
    value_valid = false;
  }
  double lowbd() const { return lower; }    // a const function member
  double upbd() const { return upper; }     // a const function member
  void changebd(double, double);            // a nonconst function member 
  double trapezoidal(int = 100) const;      // a const function member
  friend double simpson(integral, int = 100);     // a friend
};

inline void integral::changebd(double a, double b) { // definition of a fn
  lower = a;                                    // declared to be inline
  upper = b;
  value_valid = false;
}

double simpson(integral ig, int n) {            // a friend 
  double h = (ig.upper - ig.lower)/n;           // size of each subinterval
  double sum = ig.integrand(ig.lower)*0.5;
  for (int i = 1; i < n; i++) sum += ig.integrand(ig.lower + i*h);
  sum += ig.integrand(ig.upper)*0.5;

  double sum2 = 0.0; 
  for (int i = 1; i <= n; i++) sum2 += ig.integrand(ig.lower + (i-0.5)*h);

  return (sum + 4*sum2)*h/6.0;
}

double xlog(double x) { return x*x*std::log(x); }

int main(){
  integral di(0.0, 5.0, sqrt);          // initialization
  double result = di.trapezoidal(100);  // calling a fn member
  std::cout << "The integral from " << di.lowbd() << " to " 
            << di.upbd() << " is approximately = " << result << "\n";
  di.changebd(3.0,7.0);                 // change the bounds
  result = di.trapezoidal(100);  
  std::cout << "The integral from " << di.lowbd() << " to " 
            << di.upbd() << " is approximately = " << result << "\n";

  integral dj(5.0, 20, sin);
  result = dj.trapezoidal(100);  
  std::cout << "The integral from " << dj.lowbd() << " to " 
            << dj.upbd() << " is approximately = " << result << "\n";

  result = simpson(dj,100); 
  std::cout << "The integral from " << dj.lowbd() << " to " 
            << dj.upbd() << " is approximately = " << result << "\n";


  integral in(1.0,2.0,xlog);
  const integral in2(1.0,2.0,exp);
  double d = in.trapezoidal();                      // do the calculation
  double e = in.trapezoidal();                      // just returns value
         e = in.trapezoidal();                      // just returns value
         e = in2.trapezoidal();                     // do
}


double integral::trapezoidal(int n) const {       // a const member 
  if (value_valid == false || n !=  100) {
    double h = (upper - lower)/n;                 
    double sum = integrand(lower)*0.5;
    for (int i = 1; i < n; i++) sum += integrand(lower + i*h);
    sum += integrand(upper)*0.5;

    value = sum*h;                                // updating mutable member
    value_valid = true;                           // updating mutable member

    std::cout << "did the calculation\n";
  }
  return value;
}

