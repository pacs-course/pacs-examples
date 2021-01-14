# You need to change these two variables to suit your installation of muparser
# leave it blank if it is installed in standard directories
#
# If you use the dynamic library you may need to set LD_LIBRARY_PATH
MUPARSER_LIBDIR=-L$(PACS_ROOT)/lib
MUPARSERX_LIBDIR=-L$(PACS_ROOT)/lib
MUPARSERX_INCLUDE=-I$(PACS_ROOT)/include/muparserx
LIBRARY_NAME=muParserInterface
STATIC_LIBFILE=lib$(LIBRARY_NAME).a
DYNAMIC_LIBFILE=lib$(LIBRARY_NAME).so
LDLIBS+=-L. -l$(LIBRARY_NAME) $(MUPARSER_LIBDIR) -lmuparser \
-L$(MUPARSERX_LIBDIR) -lmuparserx -L$(PACS_ROOT)/lib -lpacs
CPPFLAGS+=$(MUPARSER_INCLUDE) $(MUPARSERX_INCLUDE)  
