STATIC_LIBFILE=libmmio.a
DYNAMIC_LIBFILE=libmmio.so
LDLIBS+=-L. -lmmio -Wl,-rpath=.
EIGENDIR=/usr/local/include/eigen3/
CPPFLAGS+=-I$(EIGENDIR) -I./include
