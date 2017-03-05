############################################################
#
# An example of Makefile for the course on 
# Advanced Programming for Scientific Computing
# It should be modified for adapting it to the various examples
#
############################################################
#
# The environmental variable PACS_ROOT should be set to the
# root directory where the examples reside. In practice, the directory
# where this file is found. The resolution of PACS_ROOT is made in the
# Makefile.h file, where other important variables are also set.
# The only user defined variable that must be set in this file is
# the one indicating where Makefile.h resides

MAKEFILEH_DIR=../../../
#
include $(MAKEFILEH_DIR)/Makefile.inc
#
# You may have an include file also in the current directory
# This is optional. If not present is not an error
-include Makefile.inc

#
# The general setting is as follows:
# mains are identified bt main_XX.cpp
# all other files are XX.cpp
#

all:
	@echo "make install: installs headers in Examples/include"
	@echo "make test: compiles the tests"
	@echo "make clean: cleans"

install:
	cp include/*.hpp $(PACS_INC_DIR)

test:
	(cd ./testing; $(MAKE) DEBUG=no)

clean:
	(cd ./testing; $(MAKE) clean)

distclean:
	$(MAKE) clean
	$(RM) -f ./doc
	$(RM) *.out *.bak *~
doc:
	doxygen $(DOXYFILE)

$(EXEC): $(OBJS)

$(OBJS): $(SRCS)




