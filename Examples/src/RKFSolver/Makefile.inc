LIBNAME=RKF
LDLIBS+=-L$(PACS_LIB_DIR) -lNewton
test:
	$(MAKE) clean
	$(MAKE) dynamic
	$(MAKE) static
	$(MAKE) exec
