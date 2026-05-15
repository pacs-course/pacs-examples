MAKEFILEH_DIR=../../../
DEBUG=no
include $(MAKEFILEH_DIR)/Makefile.inc
PYTHON ?= python3
MODNAME   = pacs_py
SRC     = bindings_pybind11.cpp
EXT     = $(shell $(PYTHON)-config --extension-suffix)
PYINC   = $(shell $(PYTHON)-config --includes)
CXXFLAGS  = -O3 -Wall -shared -std=c++20 -fPIC
CPPFLAGS += $(PYINC)
MODULE = $(MODNAME)$(EXT)
all: $(MODULE)

$(MODULE): $(SRC)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $(MODULE)
clean:
	rm -f $(MODNAME)*.so $(MODNAME)*.pyd
