DATADIR = ../../experiment_summary
FIGS = Distraction_Rate_4-TV Distraction_Rate_1-TV Distraction_Rate_No-TV \
       Missed_States_4-TV Missed_States_1-TV Missed_States_No-TV \
       Model_Accuracy_4-TV Model_Accuracy_1-TV Model_Accuracy_No-TV

DATS = $(addprefix $(DATADIR)/,$(addsuffix .dat,$(FIGS)))

all: .gnuplot.stamp

# A single gnuplot invocation produces all .tex + .pdf pairs, so use a
# stamp file to track whether they are up to date.
.gnuplot.stamp: plot_all.gp common.gp $(DATS)
	gnuplot plot_all.gp
	touch $@

clean:
	rm -f *.tex *.pdf .gnuplot.stamp

.PHONY: all clean
