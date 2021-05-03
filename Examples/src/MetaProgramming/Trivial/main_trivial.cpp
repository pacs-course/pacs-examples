#include <iostream>
#include <fstream>
#include <string>
#include <type_traits>
#include <cstring>
extern "C"
{
#include "Cfun.h"  //  c declarations
}
//! An aggregate 
struct Aggregate{
  double a;
  int b;
};

//! A trivially copyable class
struct TriviallyCopyable
{
  TriviallyCopyable(int a=0, double b=9):m_a(a),m_b(b){};
  int a() const {return m_a;}
  int a() {return m_a;}
  double b() const {return m_b;}
  double b() {return m_b;}
private:
  int m_a;
  double m_b;
};

//! A class with standard layout
struct StandardLayout
{
  StandardLayout(int a, double b):m_a(a),m_b(b){};
  //! Copy constructor
  /*!
    This is useless and actually wrong since this constructor
    is equivalent to the synthetic one. But declaring
    a user-defined copy constructor I am making this struct not
    satysfying the requirement of a TriviallyCopyable class (even
    if morally it is trivially copyable). But it still has standard layout.
    
    @note Don't declare (or declare it default) the copy contructor if the
    synthetic one is what you need!!
   */
  StandardLayout(StandardLayout const & rhs):m_a(rhs.m_a),m_b(rhs.m_b){};
  int a() const {return m_a;}
  int a() {return m_a;}
  double b() const {return m_b;}
  double b() {return m_b;}
private:
  int m_a;
  double m_b;
};

int main()
{
  std::cout<<std::boolalpha;
  std::cout<< " Is it trivial?"<<std::endl;
  std::cout<< " Aggregate:           "<<std::is_trivial<Aggregate>::value<<std::endl;
  std::cout<< " TriviallyCopyable:   "<<std::is_trivial<TriviallyCopyable>::value<<std::endl;
  std::cout<< " StandardLayout:      "<<std::is_trivial<StandardLayout>::value<<std::endl<<std::endl;

  std::cout<< " Is it trivially copiable?"<<std::endl;
  std::cout<< " Aggregate:           "<<std::is_trivially_copyable<Aggregate>::value<<std::endl;
  std::cout<< " TriviallyCopyable:   "<<std::is_trivially_copyable<TriviallyCopyable>::value<<std::endl;
  std::cout<< " StandardLayout:      "<<std::is_trivially_copyable<StandardLayout>::value<<std::endl<<std::endl;
  
  std::cout<< " Has it standard layout?"<<std::endl;
  std::cout<< " Agregate:            "<<std::is_standard_layout<Aggregate>::value<<std::endl;
  std::cout<< " TriviallyCopyable:   "<<std::is_standard_layout<TriviallyCopyable>::value<<std::endl;
  std::cout<< " StandardLayout:      "<<std::is_standard_layout<StandardLayout>::value<<std::endl<<std::endl;

  std::cout<< " Is it an aggregate?"<<std::endl;
  std::cout<< " Aggregate:           "<<std::is_aggregate<Aggregate>::value<<std::endl;
  std::cout<< " TriviallyCopyable:   "<<std::is_aggregate<TriviallyCopyable>::value<<std::endl;
  std::cout<< " StandardLayout:      "<<std::is_aggregate<StandardLayout>::value<<std::endl<<std::endl;




  StandardLayout sl{5,3.14};// use uniform initialization
  // In a standard layout class I can do this:
  int * psl = reinterpret_cast<int *>(&sl); // convert to pointer to first member
  // initialise with the dereferenced pointer
  StandardLayout s2=* (reinterpret_cast<StandardLayout*>(psl));
  std::cout<<"The following two values should be equal:\n";
  std::cout<<sl.a()<<" "<<sl.b()<<std::endl;
  std::cout<<s2.a()<<" "<<s2.b()<<std::endl;

  // More interesting: interfacing with a C function
  fillMyStruct(reinterpret_cast<MyStruct*>(&sl));
  std::cout<<"After calling the C function the values should be 3 and 7.:"<<std::endl;
  std::cout<<sl.a()<<" "<<sl.b()<<std::endl;

  // A trivially copyable item can be serialised in a direct way
  TriviallyCopyable tc{7,8.9};
  constexpr auto m=sizeof(TriviallyCopyable);
  // a buffer of m bytes (since c++17 I can use byte instread of char)
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
  std::cout<<tc.a()<<" "<<tc.b()<<std::endl;
  std::cout<<d.a()<<" "<<d.b()<<std::endl;
  
  // If I am using MPI I can send the TriviallyCopyable object with
  // MPI_Send(&tc,m,MPI_BYTE,1,0,MPI_COMM_WORLD);
  // and receive it as
  // MPI_Recv(&d,m,MPI_BYTE,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);

 
  
}
