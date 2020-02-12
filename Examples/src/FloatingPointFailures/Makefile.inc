%.pdf:%.tex
	$(RM) $*.aux $*.log $*.vrb $*.out
	pdflatex $<
	pdflatex $<
clean::
	$(RM) *.log *.pdf *.aux *.dvi
