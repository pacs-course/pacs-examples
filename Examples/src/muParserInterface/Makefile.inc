# You need to change these two variables to suit your installation of muparser
# leave it blank if it is installed in standard directories
#
# If you use the dynamic library you may need to set LD_LIBRARY_PATH
MUPARSER_LIBDIR=#-L/usr/local/lib
MUPARSER_INCLUDE=#-I/usr/local/include
LIBRARY_NAME=muParserInterface
STATIC_LIBFILE=lib$(LIBRARY_NAME).a
DYNAMIC_LIBFILE=lib$(LIBRARY_NAME).so
LDLIBS+=-L. -l$(LIBRARY_NAME) $(MUPARSER_LIBDIR) -lmuparser 
CPPFLAGS+=$(MUPARSER_INCLUDE) 
