# Change to suit your system 
mkTbbLib?=/usr/lib/x86_64-linux-gnu/
mkTbbInc?=/usr/include

openmp:
	$(MAKE) all CPPFLAGS+="-I. -fopenmp" CXXFLAGS+="-fopenmp" LDFLAGS=-fopenmp

parallel:
	$(MAKE) all CPPFLAGS+="-I. -DPARALLELSTD" LDLIBS+="-L${mkTbbLib} -ltbb"

