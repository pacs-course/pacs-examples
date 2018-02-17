include ../../Makefile.inc
# Chenghe to where you have your umfpack.h
SUITESPARSE_INCDIR=/usr/include/suitesparse
# Change to where you have your suistesparse libraies, like libcholdmod
SUITESPARSE_LIBDIR=/usr/lib/x86_64-linux-gnu
# Change to where you have your blas library libblas.a
BLAS_LIBDIR=/usr/lib
CPPFLAGS=-std=c++14 -I$(SUITESPARSE_INCDIR)
# If I use static libraries I need to know all dependencies of libumfpack!
static:
	$(MAKE) main "LDLIBS=-L$(SUITESPARSE_LIBDIR) -lumfpack -lsuitesparseconfig -lamd -lcholmod -lcolamd -L$(BLAS_LIBDIR) -lblas" "LDFLAGS=-static"

# If I use the (default) shared library evetithing is simpler!
shared:
	$(MAKE) main LDLIBS=-lumfpack LDFLAGS=	

main: main.o umfpack_example.o

clean:
	$(RM) main *.o
