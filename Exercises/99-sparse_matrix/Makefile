# this is not a makefile example,
# but just a memo on the simplest command line instructions

all:
	g++ -Wall -g '-DCOMPLEX=std::complex<double>' -ISparseLib/include -ISparseLib/mv/include -IIML++ -c solver.cpp
	g++ -g -Wall '-DCOMPLEX=std::complex<double>' -o solver solver.o SparseLib/lib/libsparse.a  SparseLib/lib/libspblas.a SparseLib/lib/libmv.a 
	
#g++ -ansi -g -Wall -pedantic -O3 '-DCOMPLEX=std::complex<double>'    -I../include -I../mv/include -I../../../IML++/ -c tbicg.cc 
#g++ -ansi -g -Wall -pedantic -O3 '-DCOMPLEX=std::complex<double>'   -o tbicg tbicg.o  ../lib/libsparse.a ../lib/libspblas.a ../lib/libmv.a -lm
 
	