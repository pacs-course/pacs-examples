.phony=tarfile clean default
export SHELL=/bin/bash
CXXFLAGS +=-std=c++20
tarfile:
	$(MAKE) clean
	tar cvzf CourseExamples.tgz --exclude=.svn  --exclude=*~ --exclude=*.o --exclude=*.so --exclude=*.bak Examples/

clean:
	$(RM) CourseExamples.tgz
%.pdf : %.tex
	pdflatex $<

%ps : %.dvi
	dvips $<

%dvi : %.tex
	latex $<
