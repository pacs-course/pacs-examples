#include "transposeView.hpp"
#include <iostream>
#include "Eigen/Dense"

//I need to specialize for a matrix of fleat since
//the value_type in this case is float and not the default value of double!
template <>
struct LinearAlgebra::Matrix_Traits<Eigen::Matrix3f>
{
  using value_type=float;
  using size_type=std::size_t;
};

int main()
{
  Eigen::Matrix3f m;
  m << 1, 2, 3,
    4, 5, 6,
    7, 8, 9;

  std::cout<<m<<std::endl;
  std::cout<<" Now the transpose"<<std::endl;

  LinearAlgebra::transposed_view<Eigen::Matrix3f> mt(m);

  mt(0,0)=100.f;
  
  for (auto i=0;i<3;++i)
    for (auto j=0;j<3;++j)
      {
        std::cout<<"("<<i<<", "<<j<<")= "<<mt(i,j)<<std::endl;
      }

  // and now the const version
  std::cout<<std::endl;
  const Eigen::Matrix3f mc(m);
  auto mtc=LinearAlgebra::trans(mc);
  for (auto i=0;i<3;++i)
    for (auto j=0;j<3;++j)
      {
        std::cout<<"("<<i<<", "<<j<<")= "<<mtc(i,j)<<std::endl;
      }
  //  mtc(0,1)=10.f; //Error ive if mtc is not constant the stored matrix is
  //  constant.
  
}
