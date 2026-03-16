PAPER = FLAIRS
BIB = Thesis_David_Mathews

all: $(PAPER).pdf

$(PAPER).pdf: $(PAPER).tex $(BIB).bib
	pdflatex $(PAPER)
	bibtex $(PAPER)
	pdflatex $(PAPER)
	pdflatex $(PAPER)

clean:
	rm -f $(PAPER).aux $(PAPER).bbl $(PAPER).blg $(PAPER).log $(PAPER).out

distclean: clean
	rm -f $(PAPER).pdf
