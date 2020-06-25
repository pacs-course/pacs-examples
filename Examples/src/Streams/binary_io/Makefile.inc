#you have to set the correct directory for hdf5
#make sure you have installed hdf5 or loaded the
#corresponding modules.
#
HDF5_LIB=$(mkHdf5Lib)
HDF5_INC=$(mkHdf5Inc)
LDLIBS=-L$(HDF5_LIB) -lhdf5 -L$(PACS_LIB_DIR) -lpacs
CPPFLAGS+= -I$(HDF5_INC)
CXXFLAGS=-O3 -Wall $(STDFLAGS)
