// This show shows how to eliminate compiler warnings issued because some
// parameters are not used
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

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
// Bindings of lvalue references only
void
foo(int &a)
{
  std::cout << "using void foo(int&)" << std::endl;
}

void
foo(const int &a)
{
  std::cout << "using void foo(const int&)" << std::endl;
}

// Now we consider also rvalue references
void
goo(int &a)
{
  std::cout << "using void goo(int&)" << std::endl;
}

void
goo(int &&a)
{
  std::cout << "using void goo(int&&)" << std::endl;
}

void
goo(const int &&a)
{
  std::cout << "using void goo(const int &&)" << std::endl;
}
void
goo(const int &a)
{
  std::cout << "using void goo(const int&)" << std::endl;
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
            << ". Input vector size is now " << x.size() << std::endl;
}

int
main()
{
  int  a{5};
  int &b{a};
  // int& c=createFive(); // ERROR! Cannot bind a temporary to a non-const
  // reference
  const int &   c = createFive(); // Ok lifetime of temporary is increased!
  constexpr int dcx = 2;
  std::cout << " This is an example of reference bindings. Normally you do not "
               "have all the overlads tested here!\n";
  std::cout << " Comment some overloads, recompile and see what happens!\n\n";
  // let start!
  { // block scope
    std::cout << "foo does not implement overloads with rvalue-references\n";
    std::cout << "calling foo(25) (a literal):" << std::endl;
    foo(25); // foo(const int&)
    std::cout << "calling foo(a) (a is int, a lvalue):" << std::endl;
    foo(a); // foo(int&)
    std::cout << "calling foo(b) (b is int&, a lvalue):" << std::endl;
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
    std::cout << "calling goo(25) (a literal):" << std::endl;
    goo(25); // goo(int&& a)
    std::cout << "calling goo(a) (a is int, a lvalue):" << std::endl;
    goo(a); // goo(int&)
    std::cout << "calling goo(b) (b is int&, a lvalue):" << std::endl;
    goo(b); // goo(int&)
    std::cout << "calling foo(createFive()), I am passing a rvalue:"
              << std::endl;
    goo(createFive()); // goo(int&&)
    std::cout << "calling goo(c) (c is const int &) a const lvalue:"
              << std::endl;
    goo(c); // goo(const int &)
    std::cout << "calling goo(dcx) (dcx is a constexpr):" << std::endl;
    goo(dcx); // goo(const int &)
    std::cout << "calling goo(std::move(a)) (a is an int):" << std::endl;
    goo(std::move(a)); // foo(int&& )
    std::cout << "calling goo(std::move(c)) (c is a const int&):" << std::endl;
    goo(std::move(c)); // goo(const int && )
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
  gvect(std::vector<double>(10,1.));
}
#pragma GCC diagnostic pop
