.phony=tarfile clean default
export SHELL=/bin/bash
tarfile:
	$(MAKE) clean
	tar cvzf CourseExamples.tgz --exclude=.svn  --exclude=*~ --exclude=*.o --exclude=*.so --exclude=*.bak examples/


%.pdf : %.tex
	pdflatex $<

%ps : %.dvi
	dvips $<

%dvi : %.tex
	latex $<
