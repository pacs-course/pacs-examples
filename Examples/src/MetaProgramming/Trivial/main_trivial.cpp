#include <iostream>
#include <fstream>
#include <string>
#include <type_traits>
#include <cstring>
extern "C"
{
#include "Cfun.h"  //  c declarations
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
  //! Copy constructor
  /*!
    This is useless and actually wrong since this constructor
    is equivalent to the synthetic (trivial) one. But declaring
    a user-defined copy constructor I am making this struct not
    satysfying the requirement of a TriviallyCopyable class, even
    if morally it is TriviallyCopyable
    
    @note Don't declare (or declare it default) s copy contructor if the
    synthetic one is what you need!
   */
  StandardLayout(StandardLayout const & rhs):m_a(rhs.m_a),m_b(rhs.m_b){};
  int m_a;
  double m_b;
};

int main()
{
  std::cout<<std::boolalpha;
  std::cout<< " Is it trivial?"<<std::endl;
  std::cout<< " Trivial:             "<<std::is_trivial<Trivial>::value<<std::endl;
  std::cout<< " TriviallyCopyable:   "<<std::is_trivial<TriviallyCopyable>::value<<std::endl;
  std::cout<< " StandardLayout:      "<<std::is_trivial<StandardLayout>::value<<std::endl;

  std::cout<< " Is it trivially copiable?"<<std::endl;
  std::cout<< " Trivial:             "<<std::is_trivially_copyable<Trivial>::value<<std::endl;
  std::cout<< " TriviallyCopyable:   "<<std::is_trivially_copyable<TriviallyCopyable>::value<<std::endl;
  std::cout<< " StandardLayout:      "<<std::is_trivially_copyable<StandardLayout>::value<<std::endl;
  
  std::cout<< " Has it standard layout?"<<std::endl;
  std::cout<< " Trivial:             "<<std::is_standard_layout<Trivial>::value<<std::endl;
  std::cout<< " TriviallyCopyable:   "<<std::is_standard_layout<TriviallyCopyable>::value<<std::endl;
  std::cout<< " StandardLayout:      "<<std::is_standard_layout<StandardLayout>::value<<std::endl;

  // In a standard layout class I can do this
  StandardLayout sl{5,3.14};// use uniform initialization
  int * psl = reinterpret_cast<int *>(&sl); // convert to pointer to first memeber
  // initialise with the dereferenced pointer
  StandardLayout s2=* (reinterpret_cast<StandardLayout*>(psl));
  std::cout<<"The following two values should be equal:\n";
  std::cout<<sl.m_a<<" "<<sl.m_b<<std::endl;
  std::cout<<s2.m_a<<" "<<s2.m_b<<std::endl;

  // Interfacing with a use a C function
  fillMyStruct(reinterpret_cast<MyStruct*>(&sl));
  std::cout<<"After calling the C function the values should be 3 and 7.:"<<std::endl;
  std::cout<<sl.m_a<<" "<<sl.m_b<<std::endl;

  // A trivially copyable item can be serialised in a direct way
  TriviallyCopyable tc{7,8.9};
  constexpr auto m=sizeof(TriviallyCopyable);
  // a buffer of m bytes
  char buffer[m];
  // copy into the buffer using memcpy
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
  // read into d directly
  pluto.read(reinterpret_cast<char *>(&d),m);
  std::cout<<"The following two values should be equal:\n";
  std::cout<<tc.m_a<<" "<<tc.m_b<<std::endl;
  std::cout<<d.m_a<<" "<<d.m_b<<std::endl;
  
  // If I am using MPI I can send the TriviallyCopyable object with
  // MPI_Send(&tc,m,MPI_BYTE,1,0,MPI_COMM_WORLD);
  // and receive it as
  // MPI_Recv(&d,m,MPI_BYTE,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
}
