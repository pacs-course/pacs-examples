LDLIBS=-lhdf5 -L/usr/lib/openmpi -lmpi -lmpi_cxx
CPPFLAGS+=-I/usr/lib/openmpi/include/
CXXFLAGS=-O3 -Wall $(STDFLAGS)
