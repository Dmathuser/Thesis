#ifndef _NLP_H
#define _NLP_H
#include "policy.h"

class NLP_alg : public Policy {
	private:
	int seed = 0;
	unsigned int rand_seed = 0;
	double alpha = 0.9;
	double gamma = 0.9;
	double **Qvalues;
	std::unordered_map<unsigned long, double> QNValues;
	double MSE(StateTransitionVector *Theta, StateTransitionVector *ThetaHat);
	void UpdateStateTransitionVectorTransitions(StateTransitionVector *Theta, StateTransition sas);
	void UpdateTransitions(StateTransition sas);
	void Initialize();
	double GetProbability(StateTransitionVector sas, StateCounter s);
	double GetProbability(StateCounter s, int totalCount);
	double GetProbability(StateTransitionVector *sas, int stateIndex);
	double GetProbability(int visitCount, int totalCount);
	public:
	NLP_alg(int seed, int numStates, int numActions);
	~NLP_alg();
	void setParams(double a, double g) { alpha = a; gamma = g; }
	Action getAction(State s);
	void Update(StateTransition sas);
};

#endif
