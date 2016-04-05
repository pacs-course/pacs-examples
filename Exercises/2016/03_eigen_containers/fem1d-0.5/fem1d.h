#ifndef HAVE_FEM1D_H
#define HAVE_FEM1D_H 

#include <array>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>
#include <Eigen/Dense>

std::string help_text = std::string () +
"fem1d command line options\n" +
  "-h, --help              print this text and exit\n" +
  "-a <value>              first end of interval\n" +
  "-b <value>              second end of interval\n" +
  "-n, --nnodes <value>    number of triangulation nodes\n" +
  "-m, --maxit <value>     number of iterations\n" +
  "-t, --tol <value>       tolerance\n";
  

#endif
