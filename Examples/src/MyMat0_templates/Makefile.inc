BLAS=yes# the default
# change it to suite your needs if yo do not use modules
BLAS_LIB_DIR=${mkOpenblasLib}
BLAS_INC_DIR=${mkOpenblasInc}
BLAS_LIB_NAME=openblas # if you use module you have openblas
#BLAS_LIB_NAME=blas # if you use other versions of blas

ifeq ($(BLAS),no)
  CPPFLAGS+=-DNOBLAS
  LDLIBS=-L$(PACS_LIB_DIR) -lpacs
else
  CPPFLAGS+=-I${BLAS_INC_DIR}
  LDLIBS=-L$(BLAS_LIB_DIR) -l$(BLAS_LIB_NAME) -L$(PACS_LIB_DIR) -lpacs
endif
