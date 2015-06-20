#include<iostream>
#include<fstream>
#include<cstdlib>
#include<cstdio>
#include <boost/numeric/ublas/matrix_sparse.hpp>
#include <boost/numeric/ublas/io.hpp>
#include "MMio.hpp"
#include <string>

int main(int argc, char *argv[])
{

  using namespace std;
  using namespace boost::numeric::ublas;
  using namespace MatrixMarket;
  bool toscreen(false);
  sparse_matrix<double> m;

  if (argc ==2)toscreen=true;
  string ifname;
  if (argc>0)ifname=argv[1];
  
  if (argc < 2 || ifname.empty() || ifname == "-h")
   {
     fprintf(stderr,"Usage: %s [-h] MMfilein fileout\n", argv[0]);
     exit(-1);
   }

/* Get information about the array stored in the file specified in the  */
/* argument list:                                                       */

  fprintf(stderr,"Reading matrix info from %s...\n",argv[1]);
  fstream outfile;
  if(read_MatrixMarket(ifname, m,true) ){
    if(toscreen)cout<<m;
    else{
      outfile.open(argv[2]);
      outfile<<m;
      outfile.close();
    }
  }
  else {
    cout<<"Error in reading matrix"<< string(argv[1])<< endl;
  }
  
}
