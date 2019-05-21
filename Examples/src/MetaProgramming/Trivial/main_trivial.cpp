#include <iostream>
#include <fstream>
#include <string>
#include <type_traits>
#include <cstring>
extern "C"
{
  //  c declarations
  struct MyStruct
  {
    int m_a;
    double m_b;
  };

  void fillMyStruct(struct MyStruct * s);
}
//! A trivial 
struct Trivial{
  Trivial()=default; // or you can jut omit it
  double a;
  int b;
};

struct TriviallyCopyable
{
  TriviallyCopyable(int a=0, double b=9):m_a(a),m_b(b){};
  int m_a;
  double m_b;
};

struct StandardLayout
{
  StandardLayout(int a, double b):m_a(a),m_b(b){};
  StandardLayout(StandardLayout const & rhs):m_a(rhs.m_a),m_b(rhs.m_b){};
  int m_a;
  double m_b;
};

int main()
{
  std::cout<<std::boolalpha;
  std::cout<< " Is trivial?"<<std::endl;
  std::cout<< " Trivial:             "<<std::is_trivial<Trivial>::value<<std::endl;
  std::cout<< " TriviallyCopyable:   "<<std::is_trivial<TriviallyCopyable>::value<<std::endl;
  std::cout<< " StandardLayout:      "<<std::is_trivial<StandardLayout>::value<<std::endl;

  std::cout<< " Is trivially copiable?"<<std::endl;
  std::cout<< " Trivial:             "<<std::is_trivially_copyable<Trivial>::value<<std::endl;
  std::cout<< " TriviallyCopyable:   "<<std::is_trivially_copyable<TriviallyCopyable>::value<<std::endl;
  std::cout<< " StandardLayout:      "<<std::is_trivially_copyable<StandardLayout>::value<<std::endl;
  
  std::cout<< " Has standard layout?"<<std::endl;
  std::cout<< " Trivial:             "<<std::is_standard_layout<Trivial>::value<<std::endl;
  std::cout<< " TriviallyCopyable:   "<<std::is_standard_layout<TriviallyCopyable>::value<<std::endl;
  std::cout<< " StandardLayout:      "<<std::is_standard_layout<StandardLayout>::value<<std::endl;

  // In a standard layout class I can do this
  StandardLayout sl{5,3.14};
  int * psl = reinterpret_cast<int *>(&sl);
  StandardLayout s2=* (reinterpret_cast<StandardLayout*>(psl));
  std::cout<<sl.m_a<<" "<<sl.m_b<<std::endl;
  std::cout<<s2.m_a<<" "<<s2.m_b<<std::endl;

  //! I use a C function
  fillMyStruct(reinterpret_cast<MyStruct*>(&sl));
  std::cout<<"After calling function"<<std::endl;
  std::cout<<sl.m_a<<" "<<sl.m_b<<std::endl;

  // A trivially copyable item can be serialised
  TriviallyCopyable tc{7,8.9};
  constexpr auto m=sizeof(TriviallyCopyable);
  // a buffer
  char buffer[m];
  // copy into the buffer
  std::memcpy(buffer,&tc,m);
  // open a binary file
  std::string filename("a.dat");
  std::ofstream pippo(filename,std::ios::binary);
  // write the buffer
  pippo.write(buffer,m);
  /* Alternatively, without creating a buffer 
  pippo.write(reinterpret_cast<char *>(&tc),m)
  */
  pippo.close();
  // Read from binary file
  std::ifstream pluto("a.dat",std::ios::binary);
  TriviallyCopyable d;
  pluto.read(reinterpret_cast<char *>(&d),m);
  std::cout<<tc.m_a<<" "<<tc.m_b<<std::endl;
  std::cout<<d.m_a<<" "<<d.m_b<<std::endl;
  // Note: in principle I can do the same also if the class has standard layout
  // provided I know that I am not doing deep copies.
}
