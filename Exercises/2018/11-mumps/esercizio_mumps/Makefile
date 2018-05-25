# http://MatrixProgramming.com/
# http://Evgenii.Rudnyi.Ru/

# Please define where to find include files and libraries for MUMPS
#

MPIDIR = /u/giove/MUMPS/openmpi
MUMPSDIR = /u/giove/MUMPS/MUMPS_4.10.0
INCLOC = $(MUMPSDIR)/include
MPIINCLOC = $(MPIDIR)/include

#define C++ compiler and its flags
CPP = $(MPIDIR)/bin/mpicxx
FLAGS = -O2 

F77 = $(MPIDIR)/bin/mpif77
F77FLAGS = -m32 -O2 

# define BLAS libraries (right now it is MKL)
LIBLOC = $(MUMPSDIR)/lib
LIBBLAS = -L/usr/local/lib64 -lblas 
LIBMPISEQ = -L$(MUMPSDIR)/libseq -lmpiseq
LIBSCALAPACK = /u/giove/MUMPS/scalapack/libscalapack.a 
LIBF = -lgfortran 
LIBMUMPS = -L$(MPIDIR)/lib/ -lmpi -lmpi_f77 $(MUMPSDIR)/lib/libdmumps.a $(MUMPSDIR)/lib/libmumps_common.a \
-L$(MUMPSDIR)/PORD/lib/ -lpord -L/u/giove/MUMPS/parmetis -lparmetis -lmetis \
-L/u/giove/MUMPS/ptscotch/lib -lesmumps  -lptesmumps  -lptscotch  -lptscotcherr  \
-lptscotcherrexit  -lscotch  -lscotcherr  -lscotcherrexit  

all: run_mumps_par run_mumps_par_distributed

run_mumps_par : run_mumps_par.o matrices.o
	$(CPP) $(LIBF) -o $@ $@.o matrices.o $(LIBMUMPS) $(LIBSCALAPACK) $(LIBBLAS)

run_mumps_par.o: run_mumps_par.cpp auxiliary.h matrices.h
	$(CPP) $(FLAGS) -I. -I$(INCLOC) -I$(MPIINCLOC) -c run_mumps_par.cpp

run_mumps_par_distributed : run_mumps_par_distributed.o matrices.o open_stream.o
	$(CPP) $(LIBF) -o $@ $@.o matrices.o open_stream.o $(LIBMUMPS) $(LIBSCALAPACK) $(LIBBLAS)

run_mumps_par_distributed.o: run_mumps_par_distributed.cpp auxiliary.h matrices.h
	$(CPP) $(FLAGS) -I. -I$(INCLOC) -I$(MPIINCLOC) -c run_mumps_par_distributed.cpp

matrices.o: matrices.cpp auxiliary.h matrices.h
	$(CPP) $(FLAGS) -I. -I$(INCLOC) -c matrices.cpp

open_stream.o: open_stream.f
	$(F77) $(F77FLAGS) -c open_stream.f

clean:
	rm -f *.o run_mumps run_mumps_par run_mumps_par_distributed *.solution *~

