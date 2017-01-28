#include <Eigen/SparseCore>
#include <Eigen/SparseLU>
#include "../MM_readers.hpp"
#include <iostream>
#include <string>
int main(int argc, char * argv[])
{
  using namespace Eigen;
  if (argc != 2)
    {
      std::cerr<<" Usage testMM_readers MM_filename"<<std::endl;
      return 1;
    }
  std::string matrixFile(argv[1]);
  // Some useful alias
  using SpMat=Eigen::SparseMatrix<double>;
  using SpVec=Eigen::VectorXd;
  // Read matrix
  SpMat mat=read_MM_Matrix<SpMat>(matrixFile);
  //SpVec vec=read_MM_Vector<SpVec>(vectorFile);
  SpVec e=SpVec::Ones(mat.rows());
  SpVec b=mat * e;
  SparseLU<SpMat> solver;
  solver.compute(mat);
  if(solver.info()!=Eigen::Success) {
    std::cerr<<" Decomposition failed\n";
    // decomposition failed
    return 1;
  }
  auto x = solver.solve(b);
  if(solver.info()!=Eigen::Success) {
    std::cerr<<" Solving failed\n";
    // solving failed
    return 1;
  }
  std::cout << "Error norm (should be zero): "<<(x-e).norm()<<std::endl;
  std::string matrixFileOut("./mat_out.mtx");
  std::string vectorFileOut("./vec_out.mtx");
  // From unsupported/Eigen/SparseExtra
  Eigen::saveMarket(mat, matrixFileOut);
  Eigen::saveMarketVector (x, vectorFileOut);
}
