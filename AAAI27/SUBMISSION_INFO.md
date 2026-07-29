# AAAI-27 Submission Package

Paper: `AAAI27.pdf` (built from `AAAI27.tex`). Double-blind, 7 pages.

## Key dates (all 11:59 PM UTC-12 / AoE)
- **Abstract due: July 21, 2026** (mandatory; gates the paper submission)
- **Full paper due: July 28, 2026**
- Supplementary material: July 31, 2026
- Phase-1 rejection notices: September 24, 2026
- Author feedback: October 19-25, 2026
- Final decisions: November 30, 2026
- Camera-ready: December 14, 2026
- Conference: February 16-23, 2027, Montreal

Submission site: OpenReview (AAAI.org/2027/Conference).

---

## Fields to enter for the July 21 ABSTRACT registration

### Title
Temporal PIG: A Predict-Then-Verify Approach to Resisting the Noisy-TV Problem

### Authors (entered in OpenReview; hidden from reviewers)
- David Mathews - South Dakota School of Mines and Technology
- Larry D. Pyeatt - South Dakota School of Mines and Technology

### Abstract (plain text, paste as-is)
Many curiosity-driven exploration methods in reinforcement learning are fundamentally open-loop: they reward predictions of learning without verifying that learning occurred. This makes them vulnerable to the Noisy-TV problem, where stochastic stimuli trap agents into endlessly seeking novelty from non-informative distractors. We propose a predict-then-verify principle: reward only the measured decrease in model uncertainty after each transition. We instantiate this as Temporal PIG (TPIG), a modification of Predicted Information Gain (PIG) that uses the actual change in KL-divergence as a temporal-difference reward. Evaluated on grid-world environments with varying distractor density, TPIG dramatically outperforms PIG on distraction rate, exploration speed, and model accuracy, demonstrating that closed-loop verification of learning is the key to resisting stochastic distractors.

### TL;DR (one-sentence summary)
Rewarding the measured decrease in model uncertainty after each action, rather than the predicted information gain, makes curiosity-driven exploration resistant to the Noisy-TV problem.

### Keywords
reinforcement learning; intrinsic motivation; exploration; noisy-TV problem; information gain; curiosity-driven exploration

### Suggested primary subject area
Machine Learning (ML): Reinforcement Learning

---

## Before uploading the PAPER (by July 28)

The paper is double-blind ready as built. Remaining author decisions:

1. **Code/data link.** The submission currently states the code and data "will be
   made publicly available upon publication" (the named GitHub repo was removed to
   preserve anonymity). If you want reviewers to see the code during review, set up
   an anonymized mirror (e.g. anonymous.4open.science) and add a `\begin{links}`
   block after the abstract. Otherwise leave as-is.

2. **Reproducibility checklist.** DRAFTED: `ReproducibilityChecklist.tex`
   (builds to a 2-page PDF). Answers were filled from the paper's contents.
   Before submitting, verify/decide these:
   - **4.6 code comments reference paper steps** -> answered *partial*; confirm
     against your C++ source or downgrade to *no*.
   - **4.7 seed-setting method** -> answered *no*. The paper does not describe how
     random seeds are set. One sentence in Experimental Setup would flip this to *yes*.
   - **4.8 computing infrastructure** -> answered *no*. The paper does not state
     hardware/OS/compiler. One sentence would flip this to *yes*.
   - **4.5 / code public on publication** -> answered *yes*, consistent with the
     paper's statement. Only holds if you actually release the code.
   Everything else (no theory, no external datasets, 100 runs, +/-1 SD bands,
   metrics defined, hyperparameters listed) is answered directly from the paper.

3. **PDF metadata** is already clean (no author fields; only generic TeX/pdfTeX
   producer strings). No action needed.

---

## Switching to CAMERA-READY (only if accepted)

In `AAAI27.tex`:
1. Remove the `submission` option: `\usepackage{aaai2027}` (drop `[submission]`).
2. Comment out the `Anonymous Submission` author block; uncomment the real
   author/affiliation block directly below it.
3. Restore the repository citation at the "source code and simulation data"
   sentence (a commented template is left inline in the .tex).
4. Rebuild with `make`.
