#include "../COOExtractor.hpp"
#include "../MM_readers.hpp"
#include "../Utilities.hpp"
#include <Eigen/Core>
#include <string>
#include <fstream>
int main(int argc, char * argv[])
{
  using namespace Eigen;
  // This is necessary if you wat the use the << operator on CooData objects
  // I still have to understand why. It seems that the name lookup procedure
  // is not working well on templated operators.
  using namespace LinearAlgebra;
  if (argc < 2)
    {
      std::cerr<<" Usage testMM_readers MM_filename [GraphInfoFile]"<<std::endl;
      std::cerr<<" GraphInfoFile defaults to adjacency.dat"<<std::endl;
      return 1;
    }
  std::string outFileName;
  if (argc == 2)
    {
      outFileName="adjacency.dat";
    }
  else
    {
      outFileName=argv[2];
    }
  std::string matrixFile(argv[1]);
  // Some useful alias
  using SpMat=Eigen::SparseMatrix<double>;
  // Read matrix
  SpMat A=Eigen::read_MM_Matrix<SpMat>(matrixFile);
  // COnvert matrix
  auto AC=LinearAlgebra::extractCooData(A);
  LinearAlgebra::AdjacencyGraph adGraph(AC);
  std::ofstream outFile(outFileName);
  adGraph.info(outFile,false);
  auto AR=computeRCMK(A);
  
  //std::cout<<"Prima:\n"<<A<<std::endl;
  //std::cout<<"Dopo:\n"<<AR<<std::endl;
  auto adrGraph=AdjacencyGraph{LinearAlgebra::extractCooData(AR)};
  adrGraph.info(outFile,false);
  outFile.close();
  std::ofstream originalFile("Original.mat");
  LinearAlgebra::outputForOctave(originalFile,A);
  originalFile.close();
  std::ofstream processedFile("Processed.mat");
  LinearAlgebra::outputForOctave(processedFile,AR);
  processedFile.close();
  
  /*
  using SpMatR=Eigen::SparseMatrix<double,RowMajor>;
  //  using SpMatC=Eigen::SparseMatrix<double,RowMajor>;
  std::vector<Eigen::Triplet<double>> trip;
  // Create a simple matrix
  
  trip.push_back({0,0,1.});
  trip.push_back({0,3,2.});
  trip.push_back({1,0,3.});
  trip.push_back({1,1,4.});
  trip.push_back({2,1,5.});
  trip.push_back({2,2,6.});
  trip.push_back({3,3,7.});

  SpMatR B(4,4);
  //  SpMatC B(4,4);

  B.setFromTriplets(trip.begin(),trip.end());
  // This saves spaces but it is not necessary here
  //  A.makeCompressed();
  //B.setFromTriplets(trip.begin(),trip.end());
  //B.makeCompressed();

  // Get the COO structure
  //auto AC=LinearAlgebra::extractCooData(A);
  //auto BC=LinearAlgebra::extractCooData(A);

  // print on the screen
  std::cout<<"A="<<std::endl;
  std::cout<<B<<std::endl;

  std::vector<int>indices(4);
  indices[0]=1;
  indices[1]=0;
  indices[2]=2;
  indices[3]=3;
  
  //Eigen::PermutationMatrix<Dynamic,Dynamic> P(indices);
  SpMat BP=LinearAlgebra::applySymmetricPermutation(B,indices);
  //BP=B.twistedBy(indices.asPermutation());
  std::cout<<"PA:"<<std::endl;
  std::cout<<BP<<std::endl;
  //std::cout<<"AP':"<<std::endl;
  //std::cout<<B*P<<std::endl;
  //std::cout<<"PAP':"<<std::endl;
  //std::cout<<P*B*P<<std::endl;
  //LinearAlgebra::printCooData(std::cout,AC);
  //std::cout<<AC<<std::endl;
  //std::cout<<"B="<<std::endl<<BC<<std::endl;
  */
}
