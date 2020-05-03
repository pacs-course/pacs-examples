LIBNAME=rkf
test:
	$(MAKE) clean
	$(MAKE) dynamic
	$(MAKE) static
	$(MAKE) exec
