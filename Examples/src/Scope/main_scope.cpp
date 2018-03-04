#include<iostream>
/*! A simple exercise on the concept of scope.

Are you able to guess what will be printed on the terminal?
*/

 // Global variables
int g=10;
int f=30;
// Namespace variable
namespace pippo
{
  int g=40;// it overrides ::g
  namespace pluto
  {
    int e=30; // it overrides pippo::e
    int g=15; // it overrides ::g;
  }
}
int main()
{
  using namespace std; // std names in the current scope
  int g=20; // it overrides
  int f=55;
  cout<<"Local g "<< g<<", Global g "<<::g<<endl;
  cout<<"pippo g "<< pippo::g<<", pluto g "<<pippo::pluto::g<<endl;
  {
    double f=9.9;// overrides f in the outer scope, which is now inaccessible!
    // While the global f is still accessible using full qualified name
    cout << "Local f "<<f<<", Global f "<<::f<<" Very local g"<<g<<endl;
  }
  // for loops variables are in a local scope!
  for (int g=1;g<3;++g) cout<<g<<" ";
  cout<<endl;
  
}
