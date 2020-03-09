mkTbbLib?=/opt/tbb-2020.1/build/linux_intel64_gcc_cc9.2.1_libc2.27_kernel5.3.0_release/
mkTbbInc?=/opt/tbb-2020.1/include

CPPFLAGS+=-I. -I$(PACS_INC_DIR) -I$(mkTbbInc)
LDLIBS+=-L$(PACS_LIB_DIR) -lpacs -L${mkTbbLib} -ltbb