# Change to suit your system
LIBRARY_NAME=pacs
STATIC_LIBFILE=lib$(LIBRARY_NAME).a
DYNAMIC_LIBFILE=lib$(LIBRARY_NAME).so
# We need to link with the installed libraries
LDLIBS+=-L$(PACS_LIB_DIR) -L. -lpacs

