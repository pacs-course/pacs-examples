#include<iostream>
/*! A simple exercise on the concept of scope.

Are you able to guess what will be printed on the terminal?
*/

 // Global variables
int g=10;
int f=30;
int main()
{
  using namespace std;
  int g=20;
  int f=70;
  cout<< g<<" "<<::g<<endl;
  {
    double f=9.9;
    cout << f<<" "<<::f<<" "<<g<<endl;
  }
  for (int g=1;g<3;++g) cout<<g<<" ";
  cout<<endl;
}
