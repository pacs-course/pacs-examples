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

MAKEFILEH_DIR=../../..

include $(MAKEFILEH_DIR)/Makefile.inc

.DEFAULT_GOAL = all

all: install

install:
	cp *.hpp  $(PACS_INC_DIR)

clean:
	-\rm -f $(EXEC) $(OBJS)

distclean:
	$(MAKE) clean
	-\rm -r -f ./doc *.a *.so
	-\rm -f *.out *.bak *~ $(DEPEND)

doc:
	doxygen $(DOXYFILE)






