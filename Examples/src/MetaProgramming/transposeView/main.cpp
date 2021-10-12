#include "Eigen/Dense"
#include "transposeView.hpp"
#include <iostream>

// I need to specialize the trait for a Eigen matrix of float since
// the value_type in this case is float and not the default value of double!
// I use the fact that Eigen matricies are done properly: they give information
// about the relevant types!
namespace apsc::LinearAlgebra
{
template <> struct Matrix_Traits<Eigen::Matrix3f>
{
  using value_type = Eigen::Matrix3f::Scalar;
  using size_type = Eigen::Matrix3f::Index;
};

// Remember also the const version!
template <> struct Matrix_Traits<const Eigen::Matrix3f>
{
  using value_type = Eigen::Matrix3f::Scalar;
  using size_type = Eigen::Matrix3f::Index;
};
} // namespace apsc::LinearAlgebra

int
main()
{
  Eigen::Matrix3f m;
  m << 1, 2, 3, 4, 5, 6, 7, 8, 9;

  std::cout << m << std::endl;
  std::cout << " Now change element (1,0) of the transpose" << std::endl;

  auto mt = apsc::LinearAlgebra::make_transposeView(m);

  mt(1, 0) = 100.f;
  std::cout << m << std::endl;

  // and now the const version
  std::cout << std::endl;
  const Eigen::Matrix3f mc(m);

  auto mtc = apsc::LinearAlgebra::make_transposeView(mc);
  std::cout << " Printing the the transpose" << std::endl;
  for(auto i = 0; i < 3; ++i)
    {
      for(auto j = 0; j < 3; ++j)
        {
          std::cout << mtc(i, j) << " ";
        }
      std::cout << std::endl;
    }
  // If you ancomment the following line you have a compiler error!
  // mtc(0,1)=10.f; //Error even if mtc is not constant the stored matrix is
  // constant!!

  // Now a different way to build a trasposedView, without the helper function,
  // thanks to the deduction guide The template argument is automatically
  // deduced thanks to the deduction guide!

  apsc::LinearAlgebra::TransposedView anotherView(
    m); // this is a view to a const matrix
  std::cout << "Matrix build with the deduction guide\n";
  for(auto i = 0; i < 3; ++i)
    {
      for(auto j = 0; j < 3; ++j)
        {
          std::cout << anotherView(i, j) << " ";
        }
      std::cout << std::endl;
    }
}
