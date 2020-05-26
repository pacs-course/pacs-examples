SUBDIRS = $(shell ls -d */)

all:
	for dir in $(SUBDIRS); \
	do \
	    make -C $${dir} all; \
	done

clean:
	for dir in $(SUBDIRS); \
	do \
	    make -C $${dir} clean; \
	done

distclean:
	for dir in $(SUBDIRS); \
	do \
	    make -C $${dir} distclean; \
	done
