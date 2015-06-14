#ifndef HH_Operators_HHa
#define HH_Operators_HHa
#include "expressionWrapper.hpp"
#include <utility>
namespace ET
{
  struct Add{
    static double apply(double i, double j){return i+j;}
  };
  struct Multiply{
    static double apply(double i, double j){return i*j;}
  };
  struct Subtract{
    static double apply(double i, double j){return i-j;}
  };
    
  template<class LO, class RO, class OP>
  class
  BinaryOperator : public Expr<BinaryOperator<LO,RO,OP> >
  {
  public:
    BinaryOperator(LO const & l, RO const &r):M_lo(l),M_ro(r){};
    double operator [](std::size_t i) const {return OP::apply(M_lo[i],M_ro[i]);}
    std::size_t size()const {return M_lo.size();}
  private:
    LO const & M_lo;
    RO const & M_ro;
  };

  template <class LO, class RO>
  using AddExpr= BinaryOperator<LO,RO,Add>;

  template <class LO, class RO>
  using MultExpr= BinaryOperator<LO,RO,Multiply>;

  template <class LO, class RO>
  using SubExpr= BinaryOperator<LO,RO,Subtract>;

  template <class LO, class RO>
  inline AddExpr<LO,RO> operator +(LO l, RO r){return  AddExpr<LO,RO>(l,r);}

  template <class LO, class RO>
  inline MultExpr<LO,RO> operator *(LO l, RO r){return  MultExpr<LO,RO>(l,r);}

  template <class LO, class RO>
  inline SubExpr<LO,RO> operator -(LO l, RO r){return  SubExpr<LO,RO>(l,r);}
}
#endif
