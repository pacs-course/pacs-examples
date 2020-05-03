LIBNAME=RKF
test:
	$(MAKE) clean
	$(MAKE) dynamic
	$(MAKE) static
	$(MAKE) exec
