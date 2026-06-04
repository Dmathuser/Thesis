#ifndef _NTPIG_H
#define _NTPIG_H
#include "policy.h"
// #include <vector>

class NTPIG_alg : public Policy {
	// StateTransitionVector **transitionCount[numStates][ACTION_SIZE][???]; // IS THETA_HAT
	private:
	int seed = 0;
	unsigned int rand_seed = 0;
	double alpha = 0.9;
	double gamma = 0.9;
	// std::vector<double> **Qvalues; //Q(s,a) values for action predictions. s has two components. Both must be tracked by array, but size of noise is dynamic.
	std::unordered_map<unsigned long, double> QNValues;
	//double **KL_Prior; //Value of KL_D before adding new data.
	double KL_D(StateTransitionVector *Theta, StateTransitionVector *ThetaHat);
	void UpdateStateTransitionVectorTransitions(StateTransitionVector *Theta, StateTransition sas);
	void UpdateTransitions(StateTransition sas);
	double NTPIG();
	void Initialize();
	void PrintTransitionVectors(StateTransitionVector *Theta, State s, Action a);
	double GetProbability(StateTransitionVector sas, StateCounter s);
	double GetProbability(StateCounter s, int totalCount);
	double GetProbability(StateTransitionVector *sas, int stateIndex);
	double GetProbability(int visitCount, int totalCount);
	public:
	NTPIG_alg(int seed, int numStates, int numActions);
	~NTPIG_alg();
	void setParams(double a, double g) { alpha = a; gamma = g; }
	Action getAction(State s);
	void Update(StateTransition sas);
	void PrintTransitionVectors(State s, Action a);
};

#endif
