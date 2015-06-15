#ifndef HH_VECTOREXPR_HPP
#define HH_VECTOREXPR_HPP
#include "expressionWrapper.hpp"
#include <vector>
#include <utility>
namespace ET
{
  //! A class for vectors of double
  /*!
    It is build around std::vector<double>
   */
  class Vector : public Expr<Vector>
  {
    std::vector<double> M_data;
  public:
    //! default constructor
    Vector()=default;
    //! A vector may be converted to a Vector
    explicit Vector(std::vector<double> const & v):M_data{v}{};
    //! A vector may also be moved in a Vector
    explicit Vector(std::vector<double> && v):M_data{std::move(v)}{};    
    //! Construct a Vector of n zeroes
    Vector(unsigned int n):M_data(n,0.){};
    //! Copy constructor
    Vector(Vector const &)=default;
    //! Move constructor
    Vector(Vector&&      )=default;
    //! Copy assign
    Vector & operator =(Vector const &)=default;
    //! MOve assign
    Vector & operator =(Vector&&)=default;
    //! I may build a Vector from an expression!
    template <class T>
    Vector(const Expr<T> & e):M_data()
    {
      const T & et(e);
      M_data.reserve(et.size());
      for (auto i=0; i<et.size();++i) M_data.emplace_back(et[i]);
    }
    //! Assigning an expression
    template <class T>
    Vector & operator = (const Expr<T> & e)
    {
      const T & et(e);
      M_data.clear();
      M_data.shrink_to_fit();
      M_data.reserve(et.size());
      for (auto i=0; i<et.size();++i) M_data.emplace_back(et[i]);
      return *this;
    }
    //! Returns element
    double & operator [](std::size_t i){return M_data[i];}
    double   operator [](std::size_t i) const {return M_data[i];}
    //! size of the vector
    std::size_t size()const{return M_data.size();}
    //! May be cast to a std::vector &
    /*!
      This way I can use all the methods of a std::vector!
      @code
      Vector a;
      std::vector<double> & av(a); // you cannot use {a} here!
      av.emplace_back(10.0);
      @endcode
      
     */
    operator std::vector<double> const & ()const {return M_data;}
    //! Non const version of casting operator
    operator std::vector<double>  & (){return M_data;}
    //! This does the same as casting but with a method
    /*!
      Only to show that you do not need casting operators, if you find them confusing (or if
      the compiler gets confused!)
      @code
      Vector a;
      std::vector<double> & av{a.as_vector()}; // Here you may use {}! (but () works as well!)
      av.emplace_back(10.0);
      @endcode

    */
    std::vector<double> const & as_vector()const {return M_data;}
    //! Non const version of casting operator
    std::vector<double>  & as_vector(){return M_data;}
  };

  //! I want to use range for loops with Vector objects.
  /*!
    Note the use od declval. I do not need to istantiate a vector to interrogate the type returned by begin!
   */
  inline auto begin(Vector & a)->decltype(std::declval<std::vector<double> >().begin() )
  { 
    return static_cast<std::vector<double> &>(a).begin();
  }

  inline auto end(Vector & a)->decltype(std::declval<std::vector<double> >().end() )
  { 
    return static_cast<std::vector<double>&>(a).end();
  }

  inline auto cbegin(Vector const & a)->decltype(std::declval<std::vector<double> >().cbegin() )
  { 
    return static_cast<std::vector<double> const &>(a).cbegin();
  }

  inline auto cend(Vector const & a)->decltype(std::declval<std::vector<double> >().cend() )
  { 
    return static_cast<std::vector<double> const &>(a).cend();
  }
}
#endif
