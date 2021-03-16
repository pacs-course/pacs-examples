LDLIBS+=-L$(PACS_LIB_DIR) -lpacs
Richardson.pdf : Richardson.tex
	pdflatex Richardson.tex
