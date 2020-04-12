LIBNAME=rk45
test:
	$(MAKE) clean
	$(MAKE) dynamic
	$(MAKE) static
	$(MAKE) exec
