PAPER = AAAI27

all: $(PAPER).pdf

$(PAPER).pdf: $(PAPER).tex Thesis_David_Mathews.bib
	pdflatex $(PAPER)
	bibtex $(PAPER)
	pdflatex $(PAPER)
	pdflatex $(PAPER)

clean:
	rm -f $(PAPER).aux $(PAPER).bbl $(PAPER).blg $(PAPER).log $(PAPER).out

cleanall: clean
	rm -f $(PAPER).pdf

.PHONY: all clean cleanall
