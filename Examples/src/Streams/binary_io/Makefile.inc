LDLIBS=-lhdf5 -lmpi -lmpi++
CPPFLAGS+=-I/usr/lib/openmpi/include/
CXXFLAGS=-O3 -Wall $(STDFLAGS)