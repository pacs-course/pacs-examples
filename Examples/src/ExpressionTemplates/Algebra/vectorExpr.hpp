#ifndef HH_VECTOREXPR_HPP
#define HH_VECTOREXPR_HPP
#include "expressionWrapper.hpp"
#include <vector>
#include <utility>
namespace ET
{
  class Vector : public Expr<Vector>
  {
    std::vector<double> M_data;
  public:
    Vector()=default;
    Vector(unsigned int n):M_data(n,0.){};
    Vector(Vector const &)=default;
    Vector(Vector&&      )=default;
    Vector & operator =(Vector const &)=default;
    Vector & operator =(Vector&&)=default;
    template <class T>
    Vector(const Expr<T> & e):M_data()
    {
      const T & et(e);
      M_data.resize(et.size());
      for (auto & i : M_data) i=et[i];
    }
    template <class T>
    Vector & operator = (const Expr<T> & e)
    {
      const T & et(e);
      for (auto & i : M_data) i=et[i];
      return *this;
    }
    double & operator [](std::size_t i){return M_data[i];}
    double   operator [](std::size_t i) const {return M_data[i];}
    std::size_t size()const{return M_data.size();}
    // May be cast to a std::vector &
    operator std::vector<double> const & ()const {return M_data;}
    operator std::vector<double>  & (){return M_data;}
  };

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
