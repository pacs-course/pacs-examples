CXX = g++
CC=gcc
LIB_DIR=lib
INCDIR=include
LIBDIR=lib
SRCDIR=src
INCLUDES=iohb.h mmio.h
LIBRARY=libMMio.a
OBJECTS=iohb.o mmio.o
PACKAGE=MM
LDFLAGS=-L$(LIBDIR) -lMMio
LDLIBS=-L$(LIBDIR) -lMMio
CPPFLAGS=-I$(INCDIR)
CFLAGS = -O -Wall 
CXXFLAGS = $(CFLAGS)
vpath %.hpp $(INCDIR)
vpath %.a $(LIBDIR)
vpath %.cpp $(SRCDIR)
vpath %.c $(SRCDIR)

EXEFILES=hbrhs2mtx hbmat2mtx hb2mtxstrm read_MM write_MM read_HB

default:
	echo "make library: Makes library $(LIBRARY)"
	echo "make testing. Makes some executables for testing"


library: $(LIBRARY)

$(LIBDIR)/$(LIBRARY): $(LIBDIR)/$(LIBRARY)($(OBJECTS))


testing: $(LIBRARY) $(EXEFILES)


clean :
	-rm -f *.o   $(EXEFILES)

veryclean :
	$(MAKE) clean
	-rm $(LIBDIR)/$(LIBRARY)

sample : sample.o MMio.hpp
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $@.o $(LDFLAGS)

