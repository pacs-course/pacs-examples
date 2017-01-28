// Author L. Formaggia
// Generic wrapper for reading Matrix Market files
#ifndef _MM_HEADER__
#define _MM_HEADER__
#include <string>
#include <iostream>
#include <cstdio>
#include "iohb.h"
#include "mmio.h"

//================  INTERFACES  ====================================
namespace MatrixMarket{
  template <typename BoostMatrix>
  bool read_MatrixMarket(std::string const & filename, BoostMatrix & boostMatrix, bool verbose=false);

  template <typename BoostMatrix>
  bool read_HarwellBoeing(std::string const & filename, BoostMatrix & boostMatrix);
}


//================  IMPLEMENTATIONS ====================================
namespace MatrixMarket
{
  template <typename BoostMatrix>
  bool read_MatrixMarket(std::string const & filename, BoostMatrix & boostMatrix, bool verbose)
  {
    int ret_code;
    MM_typecode matcode;
    std::FILE *f;
    int M, N, nz;   
    int i, I, J;
    double val;
    
    
    typedef typename BoostMatrix::value_type value_type;
    typedef typename BoostMatrix::size_type size_type;
    
    //Open file
    if (!(f = std::fopen(filename.c_str(), "r")))
      {
	std::cerr<<"Could not open file "<<filename<<std::endl;
	return false;
      }
    
    if (mm_read_banner(f, &matcode) != 0)
      {
	std::cerr<<"Could not process Matrix Market banner"<<std::endl;
	return false;
      }

    if (mm_is_complex(matcode)){
      std::cerr<<"This version of MatrixMarker reader does not support complex matrices"<<std::endl;
      return false;
    }

    /* find out size of sparse matrix .... */
    
    if ((ret_code = mm_read_mtx_crd_size(f, &M, &N, &nz)) !=0){
      std::cerr<<"Error while reading matrix dimensions"<<std::endl;
      return false;
    }

    if (verbose)std::cout<<"Matrix dimensions"<<M<<" "<<N<<" " <<"nnz "<<nz<<std::endl;
      
    boostMatrix.resize(size_type(M),size_type(N),false);
    boostMatrix.reserve(nz);
    
    for (i=0; i<nz; i++){
      fscanf(f, "%d %d %lg\n", &I, &J, &val);
      if (verbose && i%1==0)std::cerr<<"Read element "<<I-1<<" "<<J-1<<" "<<val<<std::endl;
      boostMatrix(size_type(I-1),size_type(J-1))=value_type(val);

    }
    
    std::fclose(f);
    return true;
  }
							  
  template <typename BoostMatrix, typename BoostVector>
  bool read_HarwellBoeing(std::string const & filename, BoostMatrix & boostMatrix, BoostVector & rHS)
  {
    int M, N, nonzeros, Nrhs;
    int *colptr, *rowind;
    double *val;
    double *rhs;
    int rhsentries;
    double *guess;
    double *exact;
    char *Type;
    char Ptrfmt[]="(13I6)";
    char Indfmt[]="(16I5)";
    char Valfmt[]="(3E26.18)";
    char Rhsfmt[]="(3E26.18)";
    int i=0;
    
    typedef typename BoostMatrix::value_type value_type;
    typedef typename BoostMatrix::size_type size_type;
    
    /* Get information about the array stored in the file specified in the  */
    /* argument list:                                                       */
    
    readHB_info(filename.c_str(), &M, &N, &nonzeros, &Type, &Nrhs);
    
    //   fprintf(stderr,"Matrix in file %s is %d x %d, with %d nonzeros with type %s;\n",
    //                       argv[1], M, N, nonzeros, Type);
    //fprintf(stderr,"%d right-hand-side(s) available.\n",Nrhs);
    
    /* Read the matrix information, generating the associated storage arrays  */ 
    // fprintf(stderr,"Reading the matrix from %s...\n",argv[1]);
    
    readHB_newmat_double(filename.c_str(), &M, &N, &nonzeros, &colptr, &rowind, &val);
    boostMatrix.resize(M,N,nonzeros);
    //    boostMatrix.reserve(nonzeros);
    
    int offset=get_HBBase();
    
    for (int j=0,k=colptr[0]-offset;(j<N && k<nonzeros);k=colptr[++j]-offset)
      for(int s=k;s<colptr[j+1]-offset;++s)
	boostMatrix(size_type(j),size_type(rowind[s]-offset))+=val[s];
    free(val);
    free(colptr);
    free(rowind);
    
    /* If a rhs is specified in the file, read one, generating the associate storage */
    if (Nrhs > 0) {
      //fprintf(stderr,"Reading right-hand-side vector(s) from %s...\n",argv[1]);
      readHB_newaux_double(filename.c_str(), 'F', &rhs);
      rHS.resize(Nrhs);
      for (int j=0;j<N;++j)rHS(j)=rhs[j];
      free(rhs);
    }
    return true;
    
  }
}
#endif

  
