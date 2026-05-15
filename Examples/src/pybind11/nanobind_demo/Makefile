MAKEFILEH_DIR=../../../
DEBUG=no
include $(MAKEFILEH_DIR)/Makefile.inc
PYTHON ?= python3
MODNAME = pacs_nb
SRC     = bindings_nanobind.cpp
EXT     = $(shell $(PYTHON)-config --extension-suffix)
PYINC   = $(shell $(PYTHON)-config --includes)
CXXFLAGS  = -O3 -Wall -shared -std=c++20 -fPIC
NBINC   = -I$(CURDIR)/include
NBSRC   = src/nb_combined.cpp
CPPFLAGS += $(PYINC) $(NBINC)
MODULE = $(MODNAME)$(EXT)

all: $(MODULE)

$(MODULE): $(SRC) $(NBSRC)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $^ -o $@ $(LDFLAGS) $(LDLIBS)
clean:
	rm -f $(MODNAME)*.so $(MODNAME)*.pyd
