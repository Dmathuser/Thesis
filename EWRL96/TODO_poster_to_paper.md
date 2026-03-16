# Converting from Poster to Full Paper

Three items that would move the paper from poster acceptance (7/10) to
full paper acceptance (8-9/10).

## 1. One non-tabular experiment (biggest impact)

Add a larger, more complex environment — e.g., a MiniGrid 16x16 with
doors, keys, and a Noisy-TV room.  Doesn't need to be deep RL; tabular
TPIG on a bigger grid still demonstrates scaling.  This addresses every
reviewer's primary concern simultaneously.

## 2. Learning-progress baseline

Add a sixth policy: tabular learning progress (reward = decrease in
prediction MSE).  Implement as a new policy class using the existing
codebase.  This directly answers the "isn't this just learning
progress?" question.

## 3. Hyperparameter ablation table

Run TPIG on one environment (e.g., 1-TV) with alpha in {0.1, 0.5, 0.9}
and gamma in {0.1, 0.5, 0.9}.  Present as a 3x3 table of final-timestep
metrics.  Addresses the "are the results robust?" question.
