#ifndef _LP_H
#define _LP_H
#include "policy.h"

class LP_alg : public Policy {
	private:
	int seed = 0;
	unsigned int rand_seed = 0;
	double alpha = 0.9;
	double gamma = 0.9;
	double **Qvalues;
	double MSE(StateTransitionVector *Theta, StateTransitionVector *ThetaHat);
	void UpdateStateTransitionVectorTransitions(StateTransitionVector *Theta, StateTransition sas);
	void UpdateTransitions(StateTransition sas);
	void Initialize();
	double GetProbability(StateTransitionVector sas, StateCounter s);
	double GetProbability(StateCounter s, int totalCount);
	double GetProbability(StateTransitionVector *sas, int stateIndex);
	double GetProbability(int visitCount, int totalCount);
	public:
	LP_alg(int seed, int numStates, int numActions);
	~LP_alg();
	void setParams(double a, double g) { alpha = a; gamma = g; }
	Action getAction(State s);
	void Update(StateTransition sas);
};

#endif
