#include <iostream> // input output
#include <fstream>// input output on file
#include <cmath> // (for sqrt)
#include <vector>
/*!
 * @defgroup GlobConst Global Constants
 *
 *   It would have been more proper to create a struct.
 *@{
 */
//! Max number of grid elements
const int   MMAX=501;
//! max number of iteration for Gauss-Siedel
const int   itermax=1000000;
//! Tolerance for stopping criterion
const double  toler=1e-8;
//! Bar length
const double L=40.;
//! First longitudinal dimension
const double a1=4.;
//! Second longitudinal dimension
const double a2=50.;
//! Dirichlet condition
const double To=46.;
//! External temperature 
const double Te=20.;
//! Conductivity
const double k=0.164;
//! Convection coefficient
const double hc=1.e-6*200.;
//! Precomputed coefficient for adimensional form of equation
const double act=2.*(a1+a2)*hc*L*L/(k*a1*a2);
/*!@}*/

/*! 
  @brief Temperature distribution in a 1D bar.

  @detail
    We solve  \f$ -T^{\prime\prime}(x)+act*(T(x)-T_e)=0, 0<x<L \f$ with 
    boundary conditions \f$ T(0)=To; T^\prime(L)=0\f$
    
    **************************************************
    Linear finite elements
    Iterative resolution by Gauss Siedel.
    **************************************************
    
    Example adapted by Luca Formaggia from  a code found in 
    "Simulation numerique an C++" di I. Danaila, F. Hecht e
    O. Pironneau.
*/
int main( )
{
  using namespace std; // avoid std::
  // Build the grid
  int M;
  do{
    cout << "Number of elements (1<N<"<<MMAX<<")"<<endl;
    cin  >> M;
  }while (M<=1 || M>=MMAX);
  
  
  double h=1./M;
  
  // Solution vector
  vector<double> theta(M+1);
  
  // Gauss Siedel is initialised with a linear vatiation
  // of T
  
  theta[0]=(To-Te)/Te;       //Condition at x=0
  for(int m=1;m <= M;m++)
    theta[m]=(1.-m*h)*(To-Te)/Te;
  
  // Gauss-Seidel
  // epsilon=||x^{k+1}-x^{k}||
  // Stopping criteria epsilon<=toler
  
  int iter=0;
  double xnew, epsilon;
     do
       { epsilon=0.;

	 // first M-1 row of linear system
         for(int m=1;m < M;m++)
         {   xnew  = (theta[m-1]+theta[m+1])/(2.+h*h*act);
          epsilon += (xnew-theta[m])*(xnew-theta[m]);
          theta[m] = xnew;
         }

	 //Last row
	 xnew = theta[M-1]; 
	 epsilon += (xnew-theta[M])*(xnew-theta[M]);
	 theta[M]=  xnew; 

	 iter=iter+1;     
       }while((sqrt(epsilon) > toler) && (iter < itermax) );

    if(iter<itermax)
      cout << "M="<<M<<"  Convergence in "<<iter<<" iterations"<<endl;
    else
      cout << "NOT CONVERGING in "<<itermax<<" iterazions "<<
	"||dx||="<<sqrt(epsilon)<<endl;

 // Analitic solution

    vector<double> thetaa(M+1);
     for(int m=0;m <= M;m++)
       thetaa[m]=Te+(To-Te)*cosh(sqrt(act)*(1-m*h))/cosh(sqrt(act));

     // writing results with format
     // x_i u_h(x_i) u(x_i)

     cout<<"Result file: result.dat"<<endl;
     ofstream f("result.dat");
       for(int m = 0; m<= M; m++)
	 // \t writes a tab 
         f<<m*h*L<<"\t"<<Te*(1.+theta[m])<<"\t"<<thetaa[m]<<endl;

     f.close();
     return 0;
}
