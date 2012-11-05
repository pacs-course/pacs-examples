DIRS=algo  alloc  concurrency  cont  contadapt  fo  i18n  io  iter  lang \
num  regex  stl  string  util
all:
	for dir in $(DIRS); do \
	$(MAKE) -C $$dir; \
             done
clean:
	for dir in $(DIRS); do \
	$(MAKE) -C $$dir clean; \
             done
