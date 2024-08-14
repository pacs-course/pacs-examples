CXXFLAGS+=-fopenmp
LDFLAGS+=-fopenmp
DEBUG=no
parallel_cpp:
	$(MAKE) CPPFLAGS+="-DCPP_PARALLEL" LDLIBS+="-L$(mkTbbLib) -ltbb" DEBUG=no
