#include <iostream>
#include <cmath>
#include <ctime>
#include <algorithm>

using namespace std;

double onenorm(double* v, int size) {       // fabs() is in <math.h> 
  // if (size > maxsize) cout << "vector size too large.\n";
  double norm = fabs(v[0]);
  for (int i = 1; i < size; i++) norm += fabs(v[i]);
  return norm;
}

double maxnorm(double* v, int size) {       // max() is in <algorithm>
  // if (size > maxsize) cout << "vector size too large.\n";
  double norm = fabs(v[0]);
  for (int i = 1; i < size; i++) norm = max(norm, fabs(v[i]));
  return norm;
}

double twonorm(double* v, int size) {
  // if (size > maxsize) cout << "vector size too large.\n";
  double norm = v[0]*v[0];
  for (int i = 1; i < size; i++) norm += v[i]*v[i];
  return sqrt(norm);
}

double twonorm2(double* v, int size) {
  // if (size > maxsize) cout << "vector size too large.\n";
  double norm = fabs(v[0]);
  for (int i = 1; i < size; i++) {
    double avi = fabs(v[i]);
    if (norm < 100 && avi < 100) norm = sqrt(norm*norm + avi*avi);
    else if (norm > avi) norm *= sqrt(1 + pow(avi/norm,2));
    else norm = avi*sqrt(1 + pow(norm/avi,2)); 
  } 
  return norm;
}

double maxnorm(double** a, int size) {
  double norm = onenorm(a[0],size);            // L 1 norm of row 0
  for (int i = 1; i < size; i++) 
    norm = max(norm, onenorm(a[i],size));      //a[i]: row i of a 
  return norm;
}

double onenorm(double** a, int size) {
  double norm = 0; 
  for (int j = 0; j < size; j++) {
    double temp = 0;                                // store column abs sum
    for (int i = 0; i < size; i++) temp += fabs(a[i][j]);
    norm = max(norm, temp);
  }
  return norm;
}

double frobnorm(double** a, int size) {
  double norm = twonorm2(a[0], size);                // L 2 norm of row 0
  for (int i = 1; i < size; i++) {
    double avi = twonorm2(a[i], size);
    if (norm < 100 && avi < 100) norm = sqrt(norm*norm + avi*avi);
    else if (norm > avi) norm *= sqrt(1 + pow(avi/norm,2));
    else norm = avi*sqrt(1 + pow(norm/avi,2)); 
  } 
  return norm;
}


double frobnorm2(double** a, int size) {
  double norm = 0;
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      double avi = fabs(a[i][j]);
      if (norm < 100 && avi < 100) norm = sqrt(norm*norm + avi*avi);
      else if (norm > avi) norm *= sqrt(1 + pow(avi/norm,2));
      else norm = avi*sqrt(1 + pow(norm/avi,2)); 
    }
  } 
  return norm;
}

double frobnorm3(double** a, int size) {
  double norm = 0;
  for (int i = 0; i < size; i++) 
    for (int j = 0; j < size; j++) norm += a[i][j]*a[i][j];
  return sqrt(norm);
}

int main() {

double** a = new double* [5];
for (int i = 0; i < 5; i++) a[i] = new double [5];
for (int i = 0; i < 5; i++) 
  for (int j = 0; j < 5; j++) 
    a[i][j] = i+j;

cout << maxnorm(a,5) << "\n";
cout << maxnorm(a[2],5) << "\n";

int dim = 5000;
//double* b = new double [dim];
double** bb = new double* [dim];
for (int i = 0; i < dim; i++) bb[i] = new double [dim];
for (int i = 0; i < dim; i++) {
//  b[i] = cos(i); 
  for (int j = 0; j < dim; j++) bb[i][j] = cos(2.0*i*i+j);
}
time_t t1 = time(0);

int many = 1;
double fb; 
for (int i = 0; i < many; i++) {
// fb = twonorm(b, dim);
  fb = frobnorm(bb, dim);
}
time_t t2 = time(0);
cout << "time in version 1 = " << difftime(t2, t1) << '\n';
cout << "fb = " << fb << '\n';

for (int i = 0; i < many; i++) {
//  fb =  twonorm2(b, dim);
 fb =  frobnorm2(bb, dim);
}
time_t t3 = time(0);
cout << "time in version 2 = " << difftime(t3, t2) << '\n';
cout << "fb = " << fb << '\n';

for (int i = 0; i < many; i++) {
  fb =  frobnorm3(bb, dim);
}
time_t t4 = time(0);
cout << "time in version 3 = " << difftime(t4, t3) << '\n';
cout << "fb = " << fb << '\n';


}
