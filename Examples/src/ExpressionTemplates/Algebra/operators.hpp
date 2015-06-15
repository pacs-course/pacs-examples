#ifndef HH_Operators_HHa
#define HH_Operators_HHa
#include "expressionWrapper.hpp"
#include <utility>
#include <cassert>
#include<iostream>
namespace ET
{
  //! The basic Addition
  struct Add{
    static double apply(double i, double j){return i+j;}
  };
  //! The basic Multiplication
  struct Multiply{
    static double apply(double i, double j){return i*j;}
  };
  //! The basic Subtraction
  struct Subtract{
    static double apply(double i, double j){return i-j;}
  };
  
  //! Binary operator expression.
  template<class LO, class RO, class OP>
  class
  BinaryOperator : public Expr<BinaryOperator<LO,RO,OP> >
  {
  public:
    BinaryOperator(LO const & l, RO const &r):M_lo(l),M_ro(r){};
    double operator [](std::size_t i) const {return OP::apply(M_lo[i],M_ro[i]);}
    std::size_t size()const 
    {
      // disabled when NDEBUG is set. Checks if both operands have the same size.
      assert(M_lo.size() == M_ro.size());
      return M_lo.size();
    }
  private:
    LO const & M_lo;
    RO const & M_ro;
  };

  //! Specialization for moltiplication by a scalar
  template<class RO, class OP>
  class
  BinaryOperator<double,RO,OP> : public Expr<BinaryOperator<double,RO,OP> >
  {
  public:
    using LO=double;
    BinaryOperator(LO const & l, RO const &r):M_lo(l),M_ro(r){};
    double operator [](std::size_t i) const {return OP::apply(M_lo,M_ro[i]);}
    std::size_t size()const 
    {
      return M_ro.size();
    }
  private:
    LO const  M_lo;
    RO const & M_ro;
  };

  //! Specialization for moltiplication by a scalar
  template<class LO, class OP>
  class
  BinaryOperator<LO,double,OP> : public Expr<BinaryOperator<LO,double,OP> >
  {
  public:
    using RO=double;
    BinaryOperator(LO const & l, RO const &r):M_lo(l),M_ro(r){};
    double operator [](std::size_t i) const {return OP::apply(M_lo[i],M_ro);}
    std::size_t size()const 
    {
      return M_lo.size();
    }
  private:
    LO const &  M_lo;
    RO const  M_ro;
  };


  template <class LO, class RO>
  using AddExpr= BinaryOperator<LO,RO,Add>;

  template <class LO, class RO>
  using MultExpr= BinaryOperator<LO,RO,Multiply>;

  template <class LO, class RO>
  using SubExpr= BinaryOperator<LO,RO,Subtract>;

  template <class LO, class RO>
  inline AddExpr<LO,RO> operator +(LO const & l, RO const & r){return  AddExpr<LO,RO>(l,r);}

  template <class LO, class RO>
  inline MultExpr<LO,RO> operator *(LO const & l, RO const & r){return  MultExpr<LO,RO>(l,r);}

  template <class LO, class RO>
  inline SubExpr<LO,RO> operator -(LO const & l, RO const & r){return  SubExpr<LO,RO>(l,r);}
}
#endif
