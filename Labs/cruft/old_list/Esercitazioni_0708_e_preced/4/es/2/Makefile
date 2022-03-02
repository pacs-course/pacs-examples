MACRO=PLUTO
DEFINES=pluto

.PHONY : all clean clean-all

all: test1.txt test2.txt test3.txt test4.txt.c test4.txt.cpp

test1.txt: test1.cpp
	g++ -E -D$(MACRO)=$(DEFINES) $^ > $@

test2.txt: test2.cpp
	g++ -E -D$(MACRO)=$(DEFINES) $^ > $@

test3.txt: test3.cpp
	g++ -E -D$(MACRO)=$(DEFINES) $^ > $@

test4.txt.c test4.txt.cpp: test4.c
#	gcc -E -D$(MACRO)=$(DEFINES) test4.c > test4.txt
	@echo Calling $(subst test4.txt.,,$@) preprocessor
	g++ -E -D$(MACRO)=$(DEFINES) $^ > $@

clean:
	for dustfile in $(wildcard *.txt*); do \
	!( test -e $$dustfile ) || rm $$dustfile; \
	done
