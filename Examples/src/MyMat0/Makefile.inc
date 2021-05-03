BLAS=yes# the default
# change it to suite your needs if yo do not use modules
# module load suitesparse will also load the blas library
RELEASE=yes
#BLAS_LIB_DIR=-L${mkOpenblasLib}
#BLAS_INC_DIR=-I${mkOpenblasInc}
#BLAS_LIB_DIR=-L/opt/atlas/lib
#BLAS_INC_DIR=-I/opt/atlas/include
BLAS_LIB_NAME=openblas # if you use module you have openblas
#BLAS_LIB_NAME=cblas # if you use module you have openblas
BLAS_LIB_NAME=blas # if you use other versions of blas

ifeq ($(BLAS),no)
  CPPFLAGS+=-DNOBLAS
  LDLIBS+=-L$(PACS_LIB_DIR) -lpacs
else
  CPPFLAGS+=${BLAS_INC_DIR}
  LDLIBS+=-L$(PACS_LIB_DIR) -lpacs $(BLAS_LIB_DIR) -l$(BLAS_LIB_NAME)
endif
export
