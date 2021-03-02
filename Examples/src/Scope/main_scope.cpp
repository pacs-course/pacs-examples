#include<iostream>
#include<vector>
/*! A simple exercise on the concept of scope.

Are you able to guess what will be printed on the terminal?
*/

 // Global variables (in the global scope)
int g=10;
int f=30;
// Namespace variable
namespace pippo
{
  int g=40;// it hides ::g
  namespace pluto
  {
    int e=30;
    int g=15; // it hides ::g;
  }
}
int main()
{
  using namespace std; // std names in the current scope
  double vector=89.97;
  std::vector<double> v={1.,2.};
  int g=20; // it overrides
  int f=55;
  cout<<"Local g="<< g<<", Global g="<<::g<<endl;
  cout<<"pippo g "<< pippo::g<<", pluto g="<<pippo::pluto::g<<endl;
  {
    double f=9.9;// overrides f in the outer scope, which is now inaccessible!
    // While the global f is still accessible using full qualified name
    cout << "Local f="<<f<<", Global f="<<::f<<" Very local g="<<g<<endl;
    // also pippo::g and pippo::pluto::g ar still accessible with teyr qulified name!
    cout << "pippo::g="<<pippo::g<<" pluto::g="<<pippo::pluto::g<<endl;

  }
  cout<<" main scope f="<<f<<endl;
  // for loops variables are in a local scope!
  for (int g=1;g<3;++g) cout<<g<<" ";
  cout<<endl;
  
  cout<<"g is equal"<<g<<endl;
  // Suggestions: use long and significant names. Use namespace to reduce risk of name clashes.

}
