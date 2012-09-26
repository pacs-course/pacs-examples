CXXFLAGS=-O3
#CXXFLAGS=-O0 # no optim 
LDFLAGS+=-lpacs -Wl,-rpath,.:$(PACS_LIB_DIR)
