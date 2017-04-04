/*!
  \file tridiag.cpp
  \brief A wrapper of lapack routines for tridiagonal lineary systems
  \author L. Formaggia
  \author D. A. Di Pietro
  \date 10-14-2005
*/

#include "tridiag.hpp"

namespace lapack{
    TriDiag::~TriDiag() {
        delete[] M_dl;
        delete[] M_d__;
        delete[] M_du;
        delete[] M_du2;
        delete[] M_ipiv;
    }


    int TriDiag::status() const {
        return M_info;
    }
}
