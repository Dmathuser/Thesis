PAPER = FLAIRS
BIB = Thesis_David_Mathews
GPDIR = gnuplot_figs
STAMP = $(GPDIR)/.gnuplot.stamp

all: $(PAPER).pdf

# Delegate figure rebuilds to the sub-make. The sub-make handles all
# dependency tracking and only touches the stamp when figures actually
# changed.
.PHONY: figures
figures:
	$(MAKE) -C $(GPDIR)

# The stamp file is maintained by the sub-make above. Empty recipe ";"
# tells make this file is built by its (phony) prerequisite.
$(STAMP): figures ;

# Re-inline figures into FLAIRS.tex when the stamp is newer than it.
# The trailing touch makes FLAIRS.tex newer than the stamp so subsequent
# runs skip the re-inline when nothing actually changed.
$(PAPER).tex: $(STAMP) inline_gnuplot_figs.py
	python3 inline_gnuplot_figs.py
	touch $@

$(PAPER).pdf: $(PAPER).tex $(BIB).bib
	pdflatex $(PAPER)
	bibtex $(PAPER)
	pdflatex $(PAPER)
	pdflatex $(PAPER)

clean:
	rm -f $(PAPER).aux $(PAPER).bbl $(PAPER).blg $(PAPER).log $(PAPER).out
	$(MAKE) -C $(GPDIR) clean

distclean: clean
	rm -f $(PAPER).pdf

.PHONY: all clean distclean
