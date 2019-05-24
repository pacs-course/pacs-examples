# http://MatrixProgramming.com/
# http://Evgenii.Rudnyi.Ru/

# Please define where to find include files and libraries for MUMPS
#

MPILIBDIR = $(mkToolchainBase)/lib
MPIINCDIR = $(mkToolchainBase)/include
MUMPSDIR = $(mkMumpsPrefix)
INCLOC = $(mkMumpsInc)

#define C++ compiler and its flags
CPP = mpic++
FLAGS = -O2 

F77 = mpif77
F77FLAGS = -O2 

# define BLAS libraries (right now it is MKL)
LIBLOC = $(mkMumpsLib)
LIBBLAS = -L$(mkOpenblasLib) -lopenblas 
LIBMPISEQ = -L$(LIBLOC)/libseq -lmpiseq

LIBF = -lgfortran 
LIBMUMPS = -L$(MUMPSDIR)/PORD/lib/ -lpord -L$(LIBLOC) -ldmumps -L$(mkScotchLib) -lesmumps -lscotcherr

all: run_mumps_par run_mumps_par_distributed

run_mumps_par : run_mumps_par.o matrices.o
	$(CPP) $(LIBF) -o $@ $@.o matrices.o open_stream.o $(LIBMUMPS)  $(LIBBLAS)

run_mumps_par.o: run_mumps_par.cpp auxiliary.h matrices.h
	$(CPP) $(FLAGS) -I. -I$(INCLOC) -I$(MPIINCDIR) -c run_mumps_par.cpp

run_mumps_par_distributed : run_mumps_par_distributed.o matrices.o open_stream.o
	$(CPP) $(LIBF) -o $@ $@.o matrices.o open_stream.o $(LIBMUMPS) $(LIBSCALAPACK) $(LIBBLAS)

run_mumps_par_distributed.o: run_mumps_par_distributed.cpp auxiliary.h matrices.h
	$(CPP) $(FLAGS) -I. -I$(INCLOC) -I$(MPIINCDIR) -c run_mumps_par_distributed.cpp

matrices.o: matrices.cpp auxiliary.h matrices.h
	$(CPP) $(FLAGS) -I. -I$(INCLOC) -c matrices.cpp

open_stream.o: open_stream.f
	$(F77) $(F77FLAGS) -c open_stream.f

clean:
	rm -f *.o run_mumps run_mumps_par run_mumps_par_distributed *.solution *~

