/*
 * main_serialization.cpp
 *
 *  Created on: Apr 13, 2020
 *      Author: forma
 */
#include <vector>
#include<iostream>
#include<set>
#include <fstream>
#include <filesystem>
#include <type_traits>

//! A trivially copy constructible class
class TrivialCopy
{
public:
  TrivialCopy()=default;
  TrivialCopy(int a, double b):a{a},b{b}{};

  int getA () const { return a;}
  void setA (int a){ this->a = a; }
  double getB () const { return b; }
  void setB (double b) { this->b = b;}

private:
  int a=0;
  double b=0.0;
};
//! A  (falsely) non-trivially copy constructible class
class NoTrivialCopy
{
public:
  NoTrivialCopy()=default;
  NoTrivialCopy(int a, double b):a{a},b{b}{};
  //! This copy-constructure is useless
  //! but makes the class no trivially-copyable
  NoTrivialCopy(NoTrivialCopy const & r):a{r.a},b{r.b}{};
  int getA () const { return a;}
  void setA (int a){ this->a = a; }
  double getB () const { return b; }
  void setB (double b) { this->b = b;}

private:
  int a=0;
  double b=0.0;
};

int main()
{
  std::filesystem::path datafile="./file.dat";
  if(!datafile.empty())
    {
      std::filesystem::remove(datafile);
      std::cout<<"Erasing "<<datafile<<std::endl;
    }

  std::ofstream binfile ("file.dat",std::ios::binary);

  int n =10;
  //an int is trivially serializable
  binfile.write(reinterpret_cast<char*>(&n), sizeof n );
  // A vector is easily serializable (but you need to store the numbe rof elements)
  // The number of elements has already been stored
  std::vector<double> v(n,10.0); // n elements
  binfile.write(reinterpret_cast<char*>(v.data()), v.size()*sizeof(double));
  // A set cannot be serialized, but I can buffer it in a vector
  std::set<int> s={1,2,3,4,5};
  std::vector<int> buf{s.begin(),s.end()}; // copy in the buffer
  int m = buf.size();
  // I need to store the number of elements
  binfile.write(reinterpret_cast<char*>(&m), sizeof m );
  binfile.write(reinterpret_cast<char*>(buf.data()), buf.size()*sizeof(int));
  buf.clear();
  buf.shrink_to_fit();// release memory
  if (std::is_trivially_copy_constructible<TrivialCopy>::value)
    {
      std::cout<<"Dumping an element of TrivialCopy\n";
      TrivialCopy tc{1,3.4};
      binfile.write(reinterpret_cast<char*>(&tc), sizeof tc );
    }
  else
    std::cout<<"TrivialCopy not trivially copy constructible (cannot serialize trivially)\n";

  if (std::is_trivially_copy_constructible<NoTrivialCopy>::value)
    {
      std::cout<<"Dumping an element of NoTrivialCopy\n";
      NoTrivialCopy tc{1,3.4};
      binfile.write(reinterpret_cast<char*>(&tc), sizeof tc );
    }
  else
    std::cout<<"NoTrivialCopy not trivially copy constructible (cannot serialize trivially)\n";
  binfile.close();
  std::cout<<"Created a file of size: "<<std::filesystem::file_size(datafile)<<std::endl;
  std::cout<<"The data stored is indeed of "<<2*sizeof(int)+v.size()*sizeof(double)+s.size()*sizeof(int)+sizeof(TrivialCopy)<<" bytes"<<std::endl;
//*****               READING BACK
// I use another file stream (input in this case)

  std::ifstream infile ("file.dat",std::ios::binary);

  // Read dimension of vector
  int l;
  infile.read(reinterpret_cast<char*>(&l),sizeof l);
  std::cout<<"Read an integer ="<<l<<std::endl;
  // To read the vector I coa usr a raw buffer with double * dp=double[l];
  // but here I show how to use vectors directly
  std::vector<double> w;
  w.resize(l);
  infile.read(reinterpret_cast<char*>(w.data()),l*sizeof(double));
  std::cout<<"vector read:\n";
  for (auto & v:w)std::cout<<v<<", ";
  std::cout<<std::endl;
  // Now the set. I overwrite l
  infile.read(reinterpret_cast<char*>(&l),sizeof l);
  std::cout<<"Read an integer ="<<l<<std::endl;
  std::vector<int> buffer;
  buffer.resize(l);
  infile.read(reinterpret_cast<char*>(buffer.data()),l*sizeof(int));
  //! dump on the set
  std::set<int> s2{buffer.begin(),buffer.end()};
  // clear buffer
  buffer.clear();
  buffer.shrink_to_fit();// release buffer memory
  std::cout<<"set read:\n";
  for (auto & v:s2)std::cout<<v<<", ";
  std::cout<<std::endl;
  // Now our class
  TrivialCopy tc;
  infile.read(reinterpret_cast<char*>(&tc),sizeof tc);
  std::cout<<" TrivialCopy read. tc1.a="<<tc.getA()<<", tc.b="<<tc.getB()<<std::endl;
  // close file
  infile.close();





}


