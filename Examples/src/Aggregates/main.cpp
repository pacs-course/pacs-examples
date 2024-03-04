#include <array>
#include <complex>
#include <iostream>
#include <set>
#include <tuple>
#include <utility>
#include <vector>
#include <initializer_list>
//! This file illustrate uniform initialization.


/* 
 * The main rules of an aggregate: all non-static data. No user-declared constructors
 * As you can see we can have methods
 *
 */
struct m
{
  int    i;
  double j;
  double mult()const {return i*j;}
};
/*!
 * @struct Derived
 * @brief Another aggregate
 *
 * This is also an aggregate since it derives from an aggregate and does not has a user-declared constructor.
 */
struct Derived : m // for struct inheritance is public by default
{
  double z = 10.; //! default value
};

/*!
 * @brief Another aggregate
 * This is an aggregate that contains an aggregate!
 */
struct ContainsM
{
  m my_m;
  double z;
};
/*!
 * @class MyClass
 * @brief A simple class
 *
 * This is *not* an aggregate, but has a non-explicit constructor that takes two
 * values. This activates bace initialization.
 *
 */
class MyClass
{
public:
  //explicit MyClass(int a, double b):i{a},j{b}{} // If explicit I get an error in the copy-initialization
  MyClass(int  a, double b) : i{a}, j{b} {}
private:
  int    i;
  double j;
};


class MyContainer
{
public:
  /*!
   * @brief constructor taking an initializer list
   *
   * I exploit the fact that a vector can be constructed with an initializer_list
   * @param l an initializer list
   */
  MyContainer(std::initializer_list<double> const & l):data(l){};
  /*!
   * @brief Assignement operator that takes an initializer_list
   *
   * @param l an initialiser_list of doubles
   * @return myself
   */
  MyContainer & operator=(std::initializer_list<double> const & l)
  {
    data=l;// a vector can be assigned to an initializer_list
    return *this;
  }
  /*!
   * @brief Pushing back a list
   *
   * To show something more elaborate
   * @param l  an initialiser_list of doubles
   * @return myself
   */
  MyContainer & push_back(std::initializer_list<double> const & l)
  {
    data.insert(data.end(),l.begin(),l.end());
    return *this;
  }

  /*!
   * @brief To show the content
   */
  void showMe() const
  {
    for (const auto &x: data)
      {
        std::cout<<x<<", ";
      }
    std::cout<<std::endl;
  }
private:
  std::vector<double> data;
};

// To avoid useless warnings for this example.
// Pragmas are a way to provide compiler directives from a source file
// Here compiling with -Wall would produce some warnings about unused variables
// With the pragma I avoid this nuisance. Here variables are unused since it is
// just an example.
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"

int
main()
{
  //** POD data
  double           c[] = {1, 2, 3};      // this was already possible in c++98
  int              j{3};                 // J is initialized to 3

  /* std::complex is not an aggregate. Brace initialization is here just a call to the constructor
     that takes teo arguments
     */
  std::complex<double> cNumber = {1.0, 3.0};// copy-initialization of a complex
  // User defined constructors.
  MyClass myc{3,4.};// direct initialization
  MyClass mycc={5,-27.};// copy-initialization. Error if constructor is explicit
  mycc={8,47.};// copy initialization. Error if constructor is explicit.
  // I can also do this: vector of 10 Myclass elements
  // initialized with {1,3.}
  std::vector<MyClass> aV(10, {1, 3.0});
  // or a vector of 2 Myclass elements
  std::vector<MyClass> aaV{{1,3.0},{9,5.0}};



 //** initializer_list initialization
  std::vector<int> v1{1, 3, 4, 5, 6};  // direct initialization of a vector<T> with an initializer list
  // the constructor is not-explicit so this is possible:
  std::vector<int> v2 ={1, 3, 4, 5, 6};  // copy initialization of a vector<T> with an initializer list
  v2={3,4,6}; // I also have copy assignment. v2 has now 3 elements
  std::set<double>   mySet{10.0, 11.0, -2.3}; // also for a set<> (in fact all standard containers)
  // with user defined class
  MyContainer mc{3.,4.};
  MyContainer mcc={4.,-4.}; //Ok since constructor is not explicit
  std::cout<<"My container contains: ";
  mcc.showMe();
  mcc={10,-9.9};// This is an assignment
  std::cout<<"My container now contains: ";
  mcc.showMe();
  mcc.push_back({4.,5.});// pushing back new elements with a initializer list
  std::cout<<"My container now contains: ";
  mcc.showMe();


  //** Aggregates: for aggregates I have brace initialization and assignment for free
  // std::pair is an aggregate
  std::pair<double, double> a{1.0, 2.0}; // This is a (direct) aggregate initialization
  a={8.0,9.0};// But I also have assignemnt for free!
   // Now for user defined stuff
  m pippo{3, 4.5}; // Aggregate intitialization
  pippo={8,9.0};// Aggregate assignement

  Derived d ={3,4.5,-2.4}; // copy-initialization of the derived class
  double z=99.99;
  Derived dd{pippo.i,pippo.j,z};// direct initialization
  Derived ddd{5,6.};// direct initialization of just i and j. z keeps its default value
  // A vector of aggregates
  std::vector<m> zz={{3,-7.0},{5,-9.0}};// list initialization combined with aggregate
  // Aggregates containing aggregates
  ContainsM contm1{{4,5.0},3};// contm1.m={4,5.}, contm1.z=3.
  ContainsM contm2= {{4,-5.0},-9.9};// contm2.m={4,-5.}, contm2.z=-9.9.
  contm2={{1,-1.0},-1};
  // Arrays are aggregate!
  // An array with 3 pairs
  // Note the two brackets... it is due to the way the std::array class
  // is defined: a std::array contains a C-array, so is an aggregate
  // containing another aggregate
   std::array<std::pair<int, double>, 3> arrayofPair{
     {{1, 4.0}, {2, 8.0}, {3, 10.0}}};
   // but if the array stores simple data you can just to
   std::array<int, 4> intArray1{1, 2, 3, 4};
   // and also
   std::array<int, 4> intArray2 = {-1, -2, -3, -4};
   // but using 2 brackest is still fine
   std::array<int, 4> intArray3{{1, 2, 3, 4}};

   // * Now a few wornings


  // Since C++17 we can sometimes avoid double brackets
  // For instance the initialization of a Derived object can be made as
  Derived d3{{8, 9.0}, 7.0};
  // but also simply as
  Derived d4{8, 9.0, 7.0};
  // This vector contains 3+4i and 5+6i
  std::vector<std::complex<double>> vc1 = {{3., 4.}, {5., 6.}};
  std::cout << "vc1=";
  for(auto i : vc1)
    std::cout << i<<" ";
  std::cout << std::endl;
  // Since there is the conversion between doble and std::complex
  // this vector contains 3+0i and 5+0i
  std::vector<std::complex<double>> vc2 = {{3}, {5.}};
  std::cout << "vc2=";
  for(auto i : vc2)
    std::cout << i<<" ";
  std::cout << std::endl;
  // BUT ALSO this vector contains 3+0i and 5_0i
  std::vector<std::complex<double>> vc3 = {3, 5.};
  std::cout << "vc3=";
  for(auto i : vc3)
    std::cout << i<<" ";
  std::cout << std::endl;
  // My advice, use always the internal brackets
  // unless you know what you are doing!

}
