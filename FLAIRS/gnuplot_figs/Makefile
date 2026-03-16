FIGS = Distraction_Rate_4-TV Distraction_Rate_1-TV Distraction_Rate_No-TV \
       Missed_States_4-TV Missed_States_1-TV Missed_States_No-TV \
       Model_Accuracy_4-TV Model_Accuracy_1-TV Model_Accuracy_No-TV

PDFS = $(addsuffix .pdf,$(FIGS))

all: $(PDFS)

# Step 1: gnuplot generates .tex and .eps for all figures at once
%.tex: plot_all.gp common.gp ../../experiment_summary/%.dat
	gnuplot plot_all.gp

# Step 2: compile each .tex to .pdf via pdflatex
# epstopdf converts the .eps to .pdf first
%.pdf: %.tex %.eps
	epstopdf $*.eps
	pdflatex -interaction=nonstopmode $<
	pdfcrop $*.pdf $*.pdf

clean:
	rm -f *.aux *.log *.eps *-eps-converted-to.pdf *.tex *.pdf
