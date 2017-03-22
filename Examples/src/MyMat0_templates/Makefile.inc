BLAS=yes# the default
ifeq ($(BLAS),no)
  CPPFLAGS+=-DNOBLAS
  LDLIBS=-L$(PACS_LIB_DIR) -lpacs
else  
  LDLIBS=-lblas -L$(PACS_LIB_DIR) -lpacs
endif
