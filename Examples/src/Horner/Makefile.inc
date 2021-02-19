mkTbbLib?=/usr/lib/x86_64-linux-gnu/
mkTbbInc?=/usr/include

CPPFLAGS+=-I. -I$(PACS_INC_DIR) -I$(mkTbbInc)
LDLIBS+=-L$(PACS_LIB_DIR) -lpacs -L${mkTbbLib} -ltbb

parallel:
	$(MAKE) all DEBUG=no CPPFLAGS+=-DPARALLELEXEC
