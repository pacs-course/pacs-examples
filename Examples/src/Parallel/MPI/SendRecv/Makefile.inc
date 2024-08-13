CXX=mpic++
CXXFLAGS+=-Wno-suggest-override -Wno-cast-function-type
DEBUG=no
parallel_cpp:
	$(MAKE) CPPFLAGS+="-DCPP_PARALLEL" LDLIBS+="-L$(mkTbbLib) -ltbb" DEBUG=no
