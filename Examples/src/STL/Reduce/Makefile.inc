LDLIBS+=-ltbb -L $(PACS_ROOT)/lib -lpacs
export RELEASE=YES
scalar:
	$(MAKE) all CPPFLAGS+=-DSCALAR
