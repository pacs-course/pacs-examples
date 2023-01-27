CXXFLAGS+=-fopenmp
LDFLAGS+=-fopenmp
DEBUG=no
parallel_cpp:
	$(MAKE) CPPFLAGS+="-DCPP_PARALLEL" LDLIBS+="-ltbb" DEBUG=no
