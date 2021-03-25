#include <vector>
#include <iostream>
using namespace std;
// An example of use of vector<T>
// This template function is just an utility
// that prints information about a vector
template <class T>
void printinfo(vector<T> const & v, char const * n)
{
  cout<<"Information on vector "<< n<<endl;
  cout<<" Size ="<<v.size()<<" Capacity= "<<v.capacity()<<endl;
  if(v.empty())cout<<"vector is empty!"<<endl;
  else{
    cout<<" Values= ";
    for (unsigned int i=0;i<v.size();++i)cout<<v[i]<<" ";
    cout<<endl;
  }
  cout<<" ** End of info"<<endl<<endl;
}

int main()
{
  vector<int> vi; //empty vector holding ints
  // size is 0 and so the capacity
  cout<<" ********** EMPTY VECTOR *****"<<endl;
  printinfo(vi,"vi");
  vi.push_back(3);// add an element
  // Now size=1 and capacity >=1
  cout<<" ********** ADDING 1 ELEMENT  *****"<<endl;
  printinfo(vi,"vi");

  cout<<" ********** ADDING 2 ELEMENTS *****"<<endl;
  vi.push_back(4);// add an element
  vi.push_back(5);// add an element
  printinfo(vi,"vi");

  // A vector element may be addressed by the [] operator
  cout<<" vi[0]="<<vi[0]<<endl; //prints 3
  vector<double> vd(3,4.0);// vector of 3 doubles initialised with 4.0
  cout<<" ********** VECTOR OF 3 DOUBLES*****"<<endl;
  printinfo(vd,"vd");
  vd[2]=6; //change an element

  // v[3]=5 ERROR element is out of range No error is emitted

  vd.at(1)=8.0;// at is another method to set vector values
  // vd.at(3)=5.0 ERROR element out of range. The program aborts
  //              printing an error message
  
  vd.push_back(5.0);// This is fine (now vd[3] contains 5).
  cout<<" ********** ADDED 1 ELEMENT *****"<<endl;
  printinfo(vd,"vd");
  vd.clear(); // clear vector. vd is now empty
  cout<<"*********** Vector vd has been cleared *****"<<endl;
  printinfo(vd,"vd");
  vector<double>(vd).swap(vd);
  cout<<"*********** Vector has been shrunk to fit size *****"<<endl;
  printinfo(vd,"vd");

  vector<int> a;
  a.reserve(5); // We have now a capacity at least 5
  cout<<"*********** A vector of capacity at least 5  *****"<<endl;
  printinfo(a,"a");
  for(int i=0 ; i<4; ++i) a.push_back(i*i);
  // I have filled a with values
  cout<<"*********** A vector of 4 int  *****"<<endl;
  printinfo(a,"a");

  typedef vector<int>::iterator Vfi; // to save typing
  // Vfi indicates an iterator to vector<float>
  // and i is here an iterator of that type
  for(Vfi i=a.begin();i<a.end();++i) *i=5; //fills a with 5

  a.resize(2);
  cout<<"*********** Vector has been resized to 2  *****"<<endl;
  printinfo(a,"a");
  
  // **************  SOME C++11 stuff  ********//
  
  for(auto & i : a) i=10; //fills a  with 10 (equivalent to previous for loop)

  a.shrink_to_fit(); // equivalent to vector<int>(a).swap(a);
  cout<<"*********** Vector has been shrunk to fit size  *****"<<endl;
  printinfo(a,"a");

  // Uniform initialization
  vector<double> kv{5., 6. ,7., 8}; // a vector with 4 components
  cout<<"*********** Vector initialised with uniform initialization syntax  *****"<<endl;
  printinfo(kv,"kv");
  
  vector<double> ku{1., 2. ,3., 4}; // a vector with 4 components

  // Swapping vectors

  ku.swap(kv); // now ku and kv are swapped
  // you may also use std::swap(ku,kv)
  cout<<"*********** After swap with another vector"<<endl;
  printinfo(kv,"kv");
  

}
