// This show shows how to eliminate compiler warnings issued because some
// parameters are not used
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"

#include <iostream>
#include <typeinfo>
#include <vector>
/*
 * This is an example to show reference bindigs. We consider the case with just
 * lvalue refernces and that with also rvalue references.
 *
 * This is an example, normally you do not activate all those overloading.
 * Actually, try to comment some and see what happens!
 *
 */
/*!
 * @brief A function returning an int
 *
 * @return an int
 */
int
createFive()
{
  int tmp{5};
  return tmp;
}

std::vector<int>
createVector()
{
  return {1, 2, 3, 4, 5};
}
/*****************************************     CASE WITHOUR R-VALUE REFENCES
 * *************************/
/*! Binds to  non const lvalues only!
 *
 * @param a the value. Being passed as (non const) l-reference I expect that the
 * function modifies  it
 */
void
foo(int &a)
{
  a = 10;
  std::cout << "using void foo(int&)" << std::endl;
}

/*!
 * It can bind to int of every category! But since I have the overload with
 * foo(int&) it binds  only to const l-values ot rvalues.
 *
 * @param a
 */

void
foo(const int &a)
{
  auto av = 5 * a; // ok i can read a
  // a=10.// Error! I cannot change a
  std::cout << "using void foo(const int&)" << std::endl;
}

/******************************************  NOW ALL POSSIBILITIES
 * **************************/
/*!
 * Binds to  non const lvalues only!
 * @param a the value. Being passed as (non const) l-reference I expect that the
 * function modifies  it
 */
void
goo(std::vector<int> &a)
{
  a.resize(2 * a.size());
  a[0] = 1000;
  std::cout << "using void goo(vector<int>&)" << std::endl;
}

/*!
 * Binds to  non const rvalues only!
 * @param a the value. Being passed as (non const) r-reference I expect that it
 * can be moved!
 */
void
goo(std::vector<int> &&a)
{
  std::vector<int> b = std::move(a);
  // now a is empty
  std::cout << "using void goo(vector<int> &&)" << std::endl;
}

/*!
 * In principle it can bind to any category. But since I have the other
 * overloads it can bind only to const lvalues!
 * @param a The vector passed as const&
 */
void
goo(const std::vector<int> &a)
{
  auto av = a[0]; // ok i can read a
  // a[0]=10.// Error! I cannot change a
  std::cout << "using void goo(const vector<int>&)" << std::endl;
}

/*!
 * Binds to  const rvalues only! This overload is normally useless.
 * It is shown here only for completeness.
 * @param a the value.
 */
void
goo(const std::vector<int> &&a)
{
  auto av = a[0];
  // ok i can read a
  // a[0]=10.// Error! I cannot change a
  std::cout << "using void goo(const vector<int> &&)" << std::endl;
}

// Here with universal references
using Vector = std::vector<double>; // to save typing

/*
// Standard way, comment if you want to use the the std::forward magic
void gvect(const Vector& v)
{
  std::cout<<"using void gvect(const T&), v size is "<<v.size()<<std::endl;
}

void gvect(Vector&& x)
{
  Vector v(std::move(x));
  std::cout<<"using void gvect(T&&), v size is "<<v.size()<<std::endl;
}
*/
//! Now the std:;forward magic: all in one!
//! Comment if you want standard bindings (and uncomment code above)
template <class T>
void
gvect(T &&x)
{
  T v = std::forward<T>(x);
  std::cout << "using void gvect<T>(T&&) with std::forward\n"
            << " created vector size is " << v.size()
            << " Input vector size is now " << x.size() << std::endl;
}

int
main()
{
  int  a{5};
  int &b{a};
  // int& c=createFive(); // ERROR! Cannot bind a temporary to a non-const
  // reference
  const int    &c = createFive(); // Ok lifetime of temporary is increased!
  constexpr int dcx = 2;
  std::cout << " This is an example of reference bindings. Normally you do not "
               "have all the overlads tested here!\n";
  std::cout << " Comment some overloads, recompile and see what happens!\n\n";
  // let start!
  { // block scope
    std::cout << "foo does not implement overloads with rvalue-references\n";
    std::cout << "calling foo(25) (a literal):" << std::endl;
    foo(25); // foo(const int&)
    std::cout << "calling foo(a) (a is int, a non-const lvalue):" << std::endl;
    foo(a); // foo(int&)
    std::cout << "calling foo(b) (b is int&, a non-const lvalue):" << std::endl;
    foo(b); // foo(int&)
    std::cout << "calling foo(createFive()), I am passing a rvalue:"
              << std::endl;
    foo(createFive()); // foo(const int&)
    std::cout << "calling foo(c) (c is const int &) a const lvalue:"
              << std::endl;
    foo(c); // foo(const int & a)
    std::cout << "calling foo(dcx) (dcx is a constexpr):" << std::endl;
    foo(dcx); // foo(const int &)
    std::cout << "calling foo(std::move(a)) (a is an int):" << std::endl;
    foo(std::move(a)); // foo(const int & )
    std::cout << "calling foo(std::move(c)) (c is a const int&):" << std::endl;
    foo(std::move(c)); // foo(const int & )

  } // end block scope

  { // block scope
    std::cout << "\ngoo implements a full set of overloads (if you have not "
                 "commented some)\n";
    auto        v = createVector();  // a vector of size 5
    auto const  cv = createVector(); // a constant vector of size 5
    auto const &rv{v};               // rv is an alias to v, but const!
    std::cout << "calling goo(v) (v is a lvalue - a vector of size 5):"
              << std::endl;
    goo(v); // goo(vector<int>&)
    std::cout << "The size of v after the call is " << v.size() << "\n";
    std::cout << "calling goo(cv) (cv is a constant vector, a const lvalue):"
              << std::endl;
    goo(cv); // goo(const vector<int>&)
    std::cout << "The size of cv after the call is " << cv.size() << "\n";
    std::cout
      << "calling goo(rv) (rv is a constant l-value ref, a const lvalue):"
      << std::endl;
    goo(rv); // goo(const vector<int>&)
    std::cout << "The size of rv after the call is " << rv.size() << "\n";
    std::cout << "calling foo(createVector()), I am passing a rvalue:"
              << std::endl;
    goo(createVector()); // goo(vector<int>&&)
    std::cout << "calling goo(std::move(v)): an r-value" << std::endl;
    goo(std::move(v)); //  goo(vector<int>&&)
    std::cout << "The size of v after the call is " << v.size() << "\n";
    std::cout << "calling goo(std::move(cv)) (cv is const, move retorns a "
                 "const rvalue):"
              << std::endl;
    goo(std::move(cv)); // goo(const vector<int> && )
    std::cout << "The size of cv after the call is " << cv.size() << "\n";
    // If you comment out the definition of goo(const vector<int> &&).
    // goo(const vector<int> &) is used instead, which is normally fine. So
    // goo(const vector<int> &&) is generally of no practicall use
  }
  std::cout << "\nNOW SOMETHING MORE INTERESTING\n";
  Vector v(100, 3.0); // a vector containing 3 of size 100

  std::cout << "calling gvect(v) (v is a vector)" << std::endl;
  gvect(v);
  std::cout << "The size of v after the call is: " << v.size() << std::endl;

  std::cout << "calling gvect(std::move(v)) (v is a Vector)" << std::endl;
  gvect(std::move(v));
  std::cout << "The size of v after the call is: " << v.size() << std::endl;

  std::cout << "I pass a vector created on the fly: gvect(vector<double>(10))"
            << std::endl;
  gvect(std::vector<double>(10, 1.));
}
#pragma GCC diagnostic pop
