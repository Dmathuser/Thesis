#ifndef _TPIG_H
#define _TPIG_H
#include "policy.h"

class TPIG_alg : public Policy {
	// StateTransitionVector **transitionCount[numStates][ACTION_SIZE][???]; // IS THETA_HAT
	private:
	int seed = 0;
	double alpha = 0.9;
	double gamma = 0.9;
	double **Qvalues; //Q(s,a) values for action predictions.
	//double **KL_Prior; //Value of KL_D before adding new data.
	double KL_D(StateTransitionVector *Theta, StateTransitionVector *ThetaHat);
	void UpdateStateTransitionVectorTransitions(StateTransitionVector *Theta, StateTransition sas);
	void UpdateTransitions(StateTransition sas);
	double TPIG();
	void Initialize();
	void PrintTransitionVectors(StateTransitionVector *Theta, State s, Action a);
	double GetProbability(StateTransitionVector sas, StateCounter s);
	double GetProbability(StateCounter s, int totalCount);
	double GetProbability(StateTransitionVector *sas, int stateIndex);
	double GetProbability(int visitCount, int totalCount);
	public:
	TPIG_alg(int seed, int numStates, int numActions);
	~TPIG_alg();
	Action getAction(State s);
	void Update(StateTransition sas);
	void PrintTransitionVectors(State s, Action a);
};

#endif
