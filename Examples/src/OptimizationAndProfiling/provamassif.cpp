#include <vector>
#include <iostream>
#include <cmath>
using namespace std;
// A stupid program to test the massif tool of valgrind
int main()
{
  vector<double>a;
  vector<int>b;
  double * c;
  cout<<" Reserving 10000 doubles: 80Kbytes"<<endl;
  a.reserve(10000);
  cout<<"Capacity ="<<a.capacity()<<endl;
  for (unsigned int i=0;i<10000;++i)a.push_back(2*i);
  cout<<" Dynamically adding another double to the vector<>"<<endl;
  a.push_back(1);
  cout<<"Capacity ="<<a.capacity()<<endl;
  // something to waste time
  for (unsigned int i=0;i<a.size();++i)a[i]=static_cast<double>(i);

  cout <<"clearing all"<<endl;
  a.clear();
  //  vector<double>().swap(a);
  a.shrink_to_fit(); // Since C++11
  cout<<" Creating 50000 ints:"<<50*sizeof(int)<<" Kbytes"<<endl;
  b.resize(50000);
  cout<<" Allocating other 10000 doubles: 80KBytes"<<endl;
  c=new double[10000];
  for (unsigned int i=0;i<b.size();++i)b[i]=2*i;
  for (unsigned int i=0;i<10000;++i)c[i]=sqrt(static_cast<double>(b[i]));
  cout<<"Deleting 10000 doubles: 80Kbytes"<<endl;
  delete [] c;
}
