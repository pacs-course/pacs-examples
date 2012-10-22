#ifndef __HH_POW_HH__
#define __HH_POW_HH__
//! A simple example of Pow for integer exponent
/* ! Note: constexpr is a C++11 extension. You need to take it away
     if you are compiling with a C++98 compiler. It is useful because
     if we give a contrant expression as argument of the apply() function
     the compiler may compute it at compile time!.

     If the argument is a non-const variable, it will operate normally.
*/
template<unsigned int N>
struct Pow{
  //! computes \f$x^N\f$.
  /*! constexpr is a C++11 extension. You may take it away or
    change it to const if you are using a non c++11 compiant compiler
   */
  static constexpr double apply(double const & x){
    return x*Pow<N-1>::apply(x);
  }
};
// Specialization for 0.
template<>
struct Pow<0>{
  static double constexpr apply(double const & x){return 1;}
};
#endif
