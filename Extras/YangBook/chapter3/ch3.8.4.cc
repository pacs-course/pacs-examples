#include<iostream>

using namespace std;

double* mxvrmy(const double** const mx, const double* const vr, int n, int m)
{
   double* tmv = new double[n];
   for (int i= 0; i < n; i++) 
   {
      tmv[i] = 0;
      for (int j=0; j<m; j++) tmv[i] += mx[i][j]*vr[j];
   }
   return tmv;
}

int main()
{
   int n=100;
   int m=200;
   double** a = new double* [n];
   for (int i=0; i<n; i++) a[i]= new double [m];
   double* b = new double [m];

   for (int i=0;i<n;i++) 
   {
      for (int j=0;j<m;j++) a[i][j]=i*i+j;
   }

   for (int j=0;j<m;j++) b[j]= 3*j+5;  

   double* c = mxvrmy(const_cast<const double** const>(a),b,n,m);

   double sum=0;
   for (int i=0; i<n;i++) sum +=c[i];
   cout << "sum = " << sum << endl;

   for (int i=0;i<n;i++) delete[] a[i];
   delete a;
   delete b;
   delete c;

   // Note:
   double** d = a;                              // Ok.
   double** const e = a;                        // Ok 

   const double** f = const_cast<const double**>(a);              // Ok 
   const double** const g = const_cast<const double** const>(a);   // Ok 

   //const double** h = a;                        // NOT Ok 
   //const double** const i = a;                  // NOT Ok 
   // T** and const T** cannot convert implicitly.

}
