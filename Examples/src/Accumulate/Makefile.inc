float:
	$(MAKE) clean
	$(MAKE) all CPPFLAGS+=-DREAL=float
extended:
	$(MAKE) clean
	$(MAKE) all CPPFLAGS+="-DREAL='long double'"
double:
	$(MAKE) clean
	$(MAKE) all

