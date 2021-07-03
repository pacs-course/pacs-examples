# Change to suit your system 
mkTbbLib?=/usr/lib/x86_64-linux-gnu/
mkTbbInc?=/usr/include

CPPFLAGS+=-I. -fopenmp
CXXFLAGS+=-fopenmp
LDLIBS+=-L${mkTbbLib} -ltbb -fopenmp -lpthread
