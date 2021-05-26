SUBDIRECTORIES=$(wildcard $(PWD)/*/.)

.PHONY=all clean

all:
	for example in $(SUBDIRECTORIES); \
	do \
	  cd $${example}; \
	  cmake . -B build; \
	  make -C build; \
	done

clean:
	for example in $(SUBDIRECTORIES); \
	do \
	  cd $${example}; \
	  rm -rf build; \
	done
