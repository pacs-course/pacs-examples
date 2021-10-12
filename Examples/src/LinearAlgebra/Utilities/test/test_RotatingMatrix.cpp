/*
 * test_RotatingMatrix.cpp
 *
 *  Created on: Dec 31, 2020
 *      Author: forma
 */
#include "RotatingMatrix.hpp"
#include <iostream>
int
main()
{
  std::cout << "Static version\n";
  {
    // apsc::RotatingMatrix<double,3,3> A;
    apsc::RotatingMatrix<double, 3, 3,
                         apsc::InsertStrategy::NewestReplacesOldest>
      A;
    using Vector = apsc::RotatingMatrix<double, 3, 3>::Vector;
    Vector v;
    for(unsigned int i = 0; i < 5; ++i)
      {
        v = i * Vector::Ones();
        A.push_back(v);
        std::cout << A.getMatrix();
        std::cout << std::endl << std::endl;
      }
    for(unsigned int i = 0; i < 3; ++i)
      {
        std::cout << "col " << i << "=\n" << A.col(i);
        std::cout << std::endl;
      }
  }
  std::cout << "Dynamic version\n";
  {
    //    apsc::RotatingMatrixX<double> A(3,3);
    apsc::RotatingMatrixX<double, apsc::InsertStrategy::NewestReplacesOldest> A(
      3, 3);
    using Vector = apsc::RotatingMatrixX<double>::Vector;
    using Vector = apsc::RotatingMatrixX<double>::Vector;
    Vector v;
    for(unsigned int i = 0; i < 5; ++i)
      {
        v = i * Vector::Ones(3);
        A.push_back(v);
        std::cout << A.getMatrix();
        std::cout << std::endl << std::endl;
      }
    for(unsigned int i = 0; i < 3; ++i)
      {
        std::cout << "col " << i << "=\n" << A.col(i);
        std::cout << std::endl;
      }
  }
}
