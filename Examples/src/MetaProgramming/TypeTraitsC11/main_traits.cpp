#include <iostream>
#include <iomanip>
#include "move_swap.hpp"
#include "fselect.hpp"
#include "../../MyMat0_enhanced/MyMat0.hpp"
struct nomove{
  nomove(double a=0):a_(a){};
  nomove(nomove const & rhs):a_(rhs.a_){}
  nomove(nomove && rhs)=delete;
  nomove & operator = (const nomove & rhs){
    this->a_=rhs.a_;
    return *this;
  }
private:
  double a_;
};

int main(){
  typedef  LinearAlgebra::MyMat0<> Matrix;
  Matrix A(10,10);
  Matrix B(10,10);
  Move_swap<Matrix> moveMatrix;
  moveMatrix(A,B);
  B(1,1)=40;
  double * p=&B(1,1);
  std::cout<<inv(p)<<std::endl;
  std::cout<<inv(B(1,1))<<std::endl;
  std::cout<<std::boolalpha;
  std::cout<<std::is_move_constructible<nomove>::value;
  nomove a(1);
  nomove b(2);
  //  Move_swap<nomove > moveNomove;

  //moveNomove(a,b);
}
