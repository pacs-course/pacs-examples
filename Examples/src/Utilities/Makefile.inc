# Change to suit your system
mkTbbLib?=/usr/lib/x86_64-linux-gnu/
mkTbbInc?=/usr/include
LIBRARY_NAME=pacs
STATIC_LIBFILE=lib$(LIBRARY_NAME).a
DYNAMIC_LIBFILE=lib$(LIBRARY_NAME).so
LDLIBS+=-L$(PACS_LIB_DIR) -L. -lpacs -L${mkTbbLib} -ltbb
CPPFLAGS+=-I$(mkTbbInc)
